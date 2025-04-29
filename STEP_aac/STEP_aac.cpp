// STEP_aac.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include "STEP_aac.h"
#include "STEPlugin.h"
#include "FileAAC.h"
#include "DlgSetup.h"
#include "..\SuperTagEditor\INI\ini.h"
//設定の読み書き
//WritePrivateProfileString はファイルが存在しない場合や、
//元ファイルが ANSI だと ANSI で文字列を書き込む
//使い辛いので STEP 本体の INI 読み書きクラスを使い回す
//UTF8/UTF16/ANSI 対応

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// メモ!
//
//   この DLL が MFC DLL に対して動的にリンクされる場合、
//   MFC 内で呼び出されるこの DLL からエクスポートされた
//   どの関数も関数の最初に追加される AFX_MANAGE_STATE
//   マクロを含んでいなければなりません。
//
//   例:
//
//   extern "C" BOOL PASCAL EXPORT ExportedFunction()
//   {
//     AFX_MANAGE_STATE(AfxGetStaticModuleState());
//     // 通常関数の本体はこの位置にあります
//   }
//
//   このマクロが各関数に含まれていること、MFC 内の
//   どの呼び出しより優先することは非常に重要です。
//   これは関数内の最初のステートメントでなければな
//   らないことを意味します、コンストラクタが MFC
//   DLL 内への呼び出しを行う可能性があるので、オブ
//   ジェクト変数の宣言よりも前でなければなりません。
//
//   詳細については MFC テクニカル ノート 33 および
//   58 を参照してください。
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_aacApp

BEGIN_MESSAGE_MAP(CSTEP_aacApp, CWinApp)
    //{{AFX_MSG_MAP(CSTEP_aacApp)
        // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
        //        この位置に生成されるコードを編集しないでください。
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_aacApp の構築

CSTEP_aacApp::CSTEP_aacApp()
{
    // TODO: この位置に構築用のコードを追加してください。
    // ここに InitInstance の中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSTEP_aacApp オブジェクト

CSTEP_aacApp theApp;

UINT nPluginID;
UINT nFileTypeMP4;
UINT nFileTypeM4V;
UINT nFileTypeM4A;

CString strINI;
bool bOptGenreListSelect;

extern "C"
int __cdecl _fseeki64_w2k(FILE *stream, __int64 offset, int whence)
{
    return _fseeki64(stream, offset, whence);
}

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return _T("Version 1.00 Copyright (C) 2004-2005 haseta\r\n")
           _T("Version 1.02M Copyright (C) 2008-2010 Mimura\r\n")
           _T("Version 1.04 Copyright (C) 2016 Kobarin\r\n")
           _T("Version 1.05 Copyright (C) 2025 haseta\r\n")
           _T("MP4(mp4,m4v,m4a)形式をサポートしています\r\n");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (Initialize() == false) return false;
    nPluginID = pID;

    // INIファイルの読み込み
    strINI = szPluginFolder;
    strINI += _T("STEP_aac.ini");
    CIniFile iniFile(strINI);

    bOptGenreListSelect = iniFile.ReadInt(_T("AAC"), _T("GenreListSelect"), 0) != 0;

    HBITMAP hM4VBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
    HBITMAP hM4ABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
    HBITMAP hMP4Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_AAC));
    nFileTypeM4A = STEPRegisterExt(nPluginID, _T("m4a"), hM4ABitmap);
    nFileTypeMP4 = STEPRegisterExt(nPluginID, _T("mp4"), hMP4Bitmap);
    nFileTypeM4V = STEPRegisterExt(nPluginID, _T("m4v"), hM4VBitmap);
    DeleteObject(hM4VBitmap);
    DeleteObject(hM4ABitmap);
    DeleteObject(hMP4Bitmap);

    return true;
}

STEP_API void WINAPI STEPFinalize() {
    Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return _T("STEP_aac");
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return true;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (!isEditSIF) return _NULL;
    switch (nColumn) {
    case COLUMN_TRACK_NAME:
    case COLUMN_ARTIST_NAME:
    case COLUMN_ALBUM_NAME:
    case COLUMN_TRACK_NUMBER:
    case COLUMN_TRACK_TOTAL:
    case COLUMN_DISC_NUMBER:
    case COLUMN_DISC_TOTAL:
    case COLUMN_YEAR:
    case COLUMN_COPYRIGHT:
    case COLUMN_COMPOSER:
    case COLUMN_KEYWORD:
    case COLUMN_SOFTWARE:
    case COLUMN_ALBM_ARTIST:
        return _EDIT;
    case COLUMN_COMMENT:
        return _MEDIT;
    case COLUMN_GENRE:
        if (bOptGenreListSelect) {
            return _CBOX;
        } else {
            return _EDIT;
        }
    //case COLUMN_DISK_NUMBER: /* STEP 045 */
    //    return _EDIT;
    case COLUMN_COMPILATION: /* STEP 049 */
        return _CBOX;
    }
    return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    switch (nColumn) { // iTunesに準拠
    case COLUMN_TRACK_NAME:     return 1024;
    case COLUMN_ARTIST_NAME:    return 1024;
    case COLUMN_ALBUM_NAME:     return 1024;
    case COLUMN_TRACK_NUMBER:   return 32;
    case COLUMN_TRACK_TOTAL:    return 32;
    case COLUMN_DISC_NUMBER:    return 32;
    case COLUMN_DISC_TOTAL:     return 32;
    case COLUMN_YEAR:           return 1024;
    case COLUMN_GENRE:          return 1024;
    case COLUMN_COMMENT:        return 1024;
    case COLUMN_COPYRIGHT:      return 1024;
    case COLUMN_COMPOSER:       return 1024;
    case COLUMN_KEYWORD:        return 1024;
    case COLUMN_SOFTWARE:       return 1024;
    case COLUMN_ALBM_ARTIST:    return 1024;
    //case COLUMN_DISK_NUMBER:	return 7; /* STEP 045 */
    case COLUMN_COMPILATION:	return 7; /* STEP 049 */
    }
    return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if ((_tcsicmp(szExt, _T("m4a")) == 0) ||
        (_tcsicmp(szExt, _T("m4v")) == 0) ||
        (_tcsicmp(szExt, _T("mp4")) == 0)) {
        extern bool LoadFileAAC(FILE_INFO *pFile);
        if (LoadFileAAC(pFileMP3) == false) {
            CString strMsg;
            strMsg.Format(_T("%s の読み込みに失敗しました"), GetFullPath(pFileMP3));
            MessageBox(NULL, strMsg, _T("AACファイルの読み込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return STEP_ERROR;
        } else {
            if(_tcsicmp(szExt, _T("m4a")) == 0){
                SetFormat(pFileMP3, nFileTypeM4A);
                SetFileTypeName(pFileMP3, _T("MP4(Audio)"));
                return STEP_SUCCESS;
            }
            if(_tcsicmp(szExt, _T("m4v")) == 0){
                SetFormat(pFileMP3, nFileTypeM4V);
                SetFileTypeName(pFileMP3, _T("MP4(Video)"));
                return STEP_SUCCESS;
            }
            if(_tcsicmp(szExt, _T("mp4")) == 0){
                SetFormat(pFileMP3, nFileTypeMP4);
                SetFileTypeName(pFileMP3, _T("MP4"));
                return STEP_SUCCESS;
            }
        }
    }
    return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    UINT nFormat = GetFormat(pFileMP3);

    if ((nFormat == nFileTypeM4A)|(nFormat == nFileTypeM4V)|(nFormat == nFileTypeMP4)) {
        extern bool WriteFileAAC(FILE_INFO *pFileMP3);
        if (WriteFileAAC(pFileMP3) == false) {
            CString strMsg;
            strMsg.Format(_T("%s の書き込みに失敗しました"), GetFullPath(pFileMP3));
            MessageBox(NULL, strMsg, _T("AACファイルの書き込み失敗"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return STEP_ERROR;
        }
        return STEP_SUCCESS;
    }
    return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CDlgSetup dlg1;
    CPropertySheet page;
    dlg1.m_bGenreListSelect = bOptGenreListSelect;
    page.AddPage(&dlg1);
    page.SetTitle(CString(STEPGetPluginName()) + _T(" オプション設定"));
    if (page.DoModal() == IDOK) {
        bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;
        CIniFile iniFile(strINI);
        iniFile.WriteInt(_T("AAC"), _T("GenreListSelect"), bOptGenreListSelect);
        iniFile.Flush();//保存実行
    }
}

STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    switch (nColumn) {
    case COLUMN_KEYWORD:
        return _T("分類");
    }
    return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return true;
}

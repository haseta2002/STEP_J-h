// CDlgMoveFolder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgMoveFolder.h"
#include "DlgNameChange.h"
#ifdef USE_STE_ORIGINAL /* STEP_J-h 004 */
#include "SHBrowseForFolder.h"
#else
#include "CIFileDialogFolderSelector.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder ダイアログ


CDlgMoveFolder::CDlgMoveFolder(CWnd* pParent /*=NULL*/)
    : COptionPage(CDlgMoveFolder::IDD)
{
    //{{AFX_DATA_INIT(CDlgMoveFolder)
    m_nFormatType = -1;
    //}}AFX_DATA_INIT
}


void CDlgMoveFolder::DoDataExchange(CDataExchange* pDX)
{
    COptionPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMoveFolder)
    DDX_Control(pDX, IDC_EDIT_INIT_PATH, m_editInitPath);
    DDX_Control(pDX, IDC_CHECK_COPY, m_chkCopy);
    DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
    DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
    DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMoveFolder, COptionPage)
    //{{AFX_MSG_MAP(CDlgMoveFolder)
    ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
    ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
    ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
    ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
    ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
    ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
    ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
    ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
    ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
    ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
    ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
    ON_BN_CLICKED(IDC_BT_DISC_NUMBER, OnBtDiscNumber)
    ON_BN_CLICKED(IDC_BT_DISC_NUMBER2, OnBtDiscNumber2)
    ON_BN_CLICKED(IDC_BT_REF, OnBtRef)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMoveFolder メッセージ ハンドラ

void CDlgMoveFolder::OnSelChangeListFormatType()
{
    // TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
    // 現在編集中の内容を反映させる
    m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_userFormatEx[m_nFormatType].bCopy = m_chkCopy.GetCheck() ? true : false;
    m_editInitPath.GetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

    m_nFormatType = m_listFormatType.GetCurSel();
    m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_chkCopy.SetCheck(m_userFormatEx[m_nFormatType].bCopy);
    m_editInitPath.SetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */
}

void CDlgMoveFolder::OutputFormatString(const TCHAR *sText)
{
    m_editFormat.ReplaceSel(sText, FALSE);
    m_editFormat.SetFocus();
}

void CDlgMoveFolder::OnBtAlbumName()
{
    OutputFormatString(_T("%ALBUM_NAME%"));
}

void CDlgMoveFolder::OnBtArtistName()
{
    OutputFormatString(_T("%ARTIST_NAME%"));
}

void CDlgMoveFolder::OnBtComment()
{
    OutputFormatString(_T("%COMMENT%"));
}

void CDlgMoveFolder::OnBtFileName()
{
    OutputFormatString(_T("%FILE_NAME%"));
}

void CDlgMoveFolder::OnBtFixString()
{
    OutputFormatString(_T("%STRING%"));
}

//void CDlgMoveFolder::OnBtNumber()
//{
//    OutputFormatString("%NUMBER%");
//}

void CDlgMoveFolder::OnBtReleaseYear()
{
    OutputFormatString(_T("%RELEASE_YEAR%"));
}

void CDlgMoveFolder::OnBtTrackName()
{
    OutputFormatString(_T("%TRACK_NAME%"));
}

void CDlgMoveFolder::OnBtTrackNumber()
{
    OutputFormatString(_T("%TRACK_NUMBER%"));
}

void CDlgMoveFolder::OnBtTrackNumber2()
{
    OutputFormatString(_T("%TRACK_NUMBER2%"));
}

void CDlgMoveFolder::OnBtDiscNumber()
{
    OutputFormatString(_T("%DISC_NUMBER%"));
}

void CDlgMoveFolder::OnBtDiscNumber2()
{
    OutputFormatString(_T("%DISC_NUMBER2%"));
}

void CDlgMoveFolder::OnBtNameChange()
{
    CDlgNameChange    dialog;

    dialog.m_strName = m_userFormatEx[m_nFormatType].strName;
    if (dialog.DoModal() == IDOK) {
        // 新しい名称に変更
        m_userFormatEx[m_nFormatType].strName = dialog.m_strName;

        // リスト更新
        OnSelChangeListFormatType();
        m_listFormatType.ResetContent();
        int i; for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
            CString strName;
            strName.Format(_T("%d: %s"), i+1, (LPCWSTR)m_userFormatEx[i].strName);
            m_listFormatType.AddString(strName);
        }
        // 選択しなおす
        m_listFormatType.SetCurSel(m_nFormatType);
    }
}

BOOL CDlgMoveFolder::OnInitDialog()
{
    // TODO: この位置に初期化の補足処理を追加してください
    COptionPage::OnInitDialog();

    int     i;
    m_listFormatType.ResetContent();
    for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
        CString strName;
        strName.Format(_T("%d: %s"), i+1, (LPCWSTR)m_userFormatEx[i].strName);
        m_listFormatType.AddString(strName);
    }

    m_listFormatType.SetCurSel(m_nFormatType);
    m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_chkCopy.SetCheck(m_userFormatEx[m_nFormatType].bCopy);
    m_editInitPath.SetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

    return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
                  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgMoveFolder::OnOK()
{
    if (!::IsWindow(m_hWnd)) return;
    m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_userFormatEx[m_nFormatType].bCopy = m_chkCopy.GetCheck() ? true : false;
    m_editInitPath.GetWindowText(m_userFormatEx[m_nFormatType].strInitFolder); /* STEP 022 */

    COptionPage::OnOK();

}

void CDlgMoveFolder::OnBtRef()
{
    // TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
    CString strFolder;
    GetDlgItemText(IDC_EDIT_INIT_PATH, strFolder);

    // フォルダ選択ダイアログを開く
    TCHAR    sFolderName[_MAX_PATH] = {'\0'};
    _tcsncpy_s(sFolderName, strFolder, _TRUNCATE);

#ifdef USE_STE_ORIGINAL /* STEP_J-h 004 */
    CSHBrowseForFolder    browse;
#else
    CIFileDialogFolderSelector browse;
#endif
    browse.SetEnableSubDirButton(false);
    if (browse.Exec(sFolderName, _MAX_PATH)) {
        SetDlgItemText(IDC_EDIT_INIT_PATH, sFolderName);
    }
}

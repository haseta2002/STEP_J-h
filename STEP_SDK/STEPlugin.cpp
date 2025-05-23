#include "stdafx.h"
#include "windows.h"

#include "STEPlugin.h"

HINSTANCE g_hInstance = NULL;
HINSTANCE hLib = NULL;

/* プラグインから呼び出せる関数 */
extern "C" {
UINT (WINAPI *STEPGetCommandID)(void);
HMENU (WINAPI *STEPGetMenu)(UINT);
void (WINAPI *STEPAddToolBarButton)(HBITMAP, UINT, TCHAR*);
UINT (WINAPI *STEPRegisterExt)(UINT, LPCTSTR, HBITMAP);
UINT (WINAPI *STEPKeyAssign)(UINT, LPCTSTR, LPCTSTR);
const TCHAR* (WINAPI *STEPGetGenreNameSIF)(BYTE byGenre);
BYTE (WINAPI *STEPGetGenreCode)(const TCHAR* szGenre);
bool (WINAPI *STEPIsUserGenre)(const TCHAR* szGenre);
int (WINAPI *STEPGetNumericTrackNumber)(const TCHAR* szTrackNumber, TCHAR* szNumericNumber, int size); /* STEP 037 */
int (WINAPI *STEPGetIntegerTrackNumber)(const TCHAR* szTrackNumber); /* STEP 037 */
int (WINAPI *STEPGetNumericDiscNumber)(const TCHAR* szDiscNumber, TCHAR* szNumericNumber, int size); /* STEP 037 */
int (WINAPI *STEPGetIntegerDiscNumber)(const TCHAR* szDiscNumber); /* STEP 037 */

bool (WINAPI *STEPGetBooleanValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetBooleanValue)(FILE_INFO*, UINT, bool);
long (WINAPI *STEPGetLongValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetLongValue)(FILE_INFO*, UINT, long);
int (WINAPI *STEPGetIntValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetIntValue)(FILE_INFO*, UINT, int);
void* (WINAPI *STEPGetVoidValue)(FILE_INFO*, UINT);
void (WINAPI *STEPSetVoidValue)(FILE_INFO*, UINT, void*);
LPCTSTR (WINAPI *STEPGetValue)(FILE_INFO*, FIELDTYPE);
void (WINAPI *STEPSetValue)(FILE_INFO*, FIELDTYPE, LPCTSTR);
void (WINAPI *STEPProcessSelectedFiles)(LPCTSTR, STEPProcessSelectedFilesCB);
void (WINAPI *STEPProcessSelectedFilesForUpdate)(LPCTSTR, STEPProcessSelectedFilesCB);

bool (WINAPI *STEPIsRangeSelected)(void);
int (WINAPI *STEPGetSelectedCount)(void);
int (WINAPI *STEPGetSelectedItem)(void);
bool (WINAPI *STEPIsCurrentCellEditOK)(void);
bool (WINAPI *STEPItemHasChildren)(int);
bool (WINAPI *STEPGetSelectedRange)(int*, int*, int* , int*);
bool (WINAPI *STEPIsItemFile)(int);
void (WINAPI *STEPChangeSubItemText)(int nItem, int nColumn, LPCTSTR szText);
const TCHAR* (WINAPI *STEPGetSubItemText)(int nItem, int nColumn);
void (WINAPI *STEPGetFileInfo)(int nItem, FILE_INFO* info);
UINT (WINAPI *STEPGETColumnType)(int nColumn);
bool (WINAPI *STEPIsNumeric)(const TCHAR* szText); /* STEP 037 */
void (WINAPI *STEPConvSiFieldToId3tag)(FILE_INFO* pFileInfo); /* STEP 037 */

void (WINAPI *STEPInitDataSIF)(FILE_INFO* info);
void (WINAPI *STEPInitDataID3)(FILE_INFO* info); /* STEP 029 */
void (WINAPI *STEPInitData)(FILE_INFO* info);
bool (WINAPI *STEPFileNameChange)(FILE_INFO* pFileInfo, LPCTSTR szNewFileName);
bool (WINAPI *STEPWriteTag)(FILE_INFO* pFileInfo);

bool (WINAPI *STEPUpdateCellInfo)(void);
}

bool Initialize(void)
{
    hLib = LoadLibrary(_T("SuperTagEditor.exe"));
    if(hLib == NULL) return false;
    (FARPROC&)STEPGetCommandID = GetProcAddress(hLib, "STEPGetCommandID");
    (FARPROC&)STEPGetMenu = GetProcAddress(hLib, "STEPGetMenu");
    (FARPROC&)STEPAddToolBarButton = GetProcAddress(hLib, "STEPAddToolBarButton");
    (FARPROC&)STEPRegisterExt = GetProcAddress(hLib, "STEPRegisterExt");
    (FARPROC&)STEPKeyAssign = GetProcAddress(hLib, "STEPKeyAssign");
    (FARPROC&)STEPGetGenreNameSIF = GetProcAddress(hLib, "STEPGetGenreNameSIF");
    (FARPROC&)STEPGetGenreCode = GetProcAddress(hLib, "STEPGetGenreCode");
    (FARPROC&)STEPIsUserGenre = GetProcAddress(hLib, "STEPIsUserGenre");
    (FARPROC&)STEPGetNumericTrackNumber = GetProcAddress(hLib, "STEPGetNumericTrackNumber"); /* STEP 037 */
    (FARPROC&)STEPGetIntegerTrackNumber = GetProcAddress(hLib, "STEPGetIntegerTrackNumber"); /* STEP 037 */
    (FARPROC&)STEPGetNumericDiscNumber = GetProcAddress(hLib, "STEPGetNumericDiscNumber"); /* STEP 037 */
    (FARPROC&)STEPGetIntegerDiscNumber = GetProcAddress(hLib, "STEPGetIntegerDiscNumber"); /* STEP 037 */

    (FARPROC&)STEPGetBooleanValue = GetProcAddress(hLib, "STEPGetBooleanValue");
    (FARPROC&)STEPSetBooleanValue = GetProcAddress(hLib, "STEPSetBooleanValue");
    (FARPROC&)STEPGetLongValue = GetProcAddress(hLib, "STEPGetLongValue");
    (FARPROC&)STEPSetLongValue = GetProcAddress(hLib, "STEPSetLongValue");
    (FARPROC&)STEPGetIntValue = GetProcAddress(hLib, "STEPGetIntValue");
    (FARPROC&)STEPSetIntValue = GetProcAddress(hLib, "STEPSetIntValue");
    (FARPROC&)STEPGetVoidValue = GetProcAddress(hLib, "STEPGetVoidValue");
    (FARPROC&)STEPSetVoidValue = GetProcAddress(hLib, "STEPSetVoidValue");
    (FARPROC&)STEPGetValue = GetProcAddress(hLib, "STEPGetValue");
    (FARPROC&)STEPSetValue = GetProcAddress(hLib, "STEPSetValue");
    (FARPROC&)STEPProcessSelectedFiles = GetProcAddress(hLib, "STEPProcessSelectedFiles");
    (FARPROC&)STEPProcessSelectedFilesForUpdate = GetProcAddress(hLib, "STEPProcessSelectedFilesForUpdate");

    (FARPROC&)STEPIsRangeSelected = GetProcAddress(hLib, "STEPIsRangeSelected");
    (FARPROC&)STEPGetSelectedCount = GetProcAddress(hLib, "STEPGetSelectedCount");
    (FARPROC&)STEPGetSelectedItem = GetProcAddress(hLib, "STEPGetSelectedItem");
    (FARPROC&)STEPIsCurrentCellEditOK = GetProcAddress(hLib, "STEPIsCurrentCellEditOK");
    (FARPROC&)STEPItemHasChildren = GetProcAddress(hLib, "STEPItemHasChildren");
    (FARPROC&)STEPGetSelectedRange = GetProcAddress(hLib, "STEPGetSelectedRange");
    (FARPROC&)STEPIsItemFile = GetProcAddress(hLib, "STEPIsItemFile");
    (FARPROC&)STEPChangeSubItemText = GetProcAddress(hLib, "STEPChangeSubItemText");
    (FARPROC&)STEPGetSubItemText = GetProcAddress(hLib, "STEPGetSubItemText");
    (FARPROC&)STEPGetFileInfo = GetProcAddress(hLib, "STEPGetFileInfo");
    (FARPROC&)STEPGETColumnType = GetProcAddress(hLib, "STEPGETColumnType");
    (FARPROC&)STEPIsNumeric = GetProcAddress(hLib, "STEPIsNumeric"); /* STEP 037 */
    (FARPROC&)STEPConvSiFieldToId3tag = GetProcAddress(hLib, "STEPConvSiFieldToId3tag"); /* STEP 037 */

    (FARPROC&)STEPInitDataSIF = GetProcAddress(hLib, "STEPInitDataSIF");
    (FARPROC&)STEPInitDataID3 = GetProcAddress(hLib, "STEPInitDataID3"); /* STEP 029 */
    (FARPROC&)STEPInitData = GetProcAddress(hLib, "STEPInitData");
    (FARPROC&)STEPFileNameChange = GetProcAddress(hLib, "STEPFileNameChange");
    (FARPROC&)STEPWriteTag = GetProcAddress(hLib, "STEPWriteTag");

    (FARPROC&)STEPUpdateCellInfo = GetProcAddress(hLib, "STEPUpdateCellInfo");

    return true;
}

void Finalize(void) {
    if (hLib != NULL) {
        FreeLibrary(hLib);
        hLib = NULL;
    }
}

LPCTSTR    GetValue(FILE_INFO* pInfo, FIELDTYPE nField) {
    return STEPGetValue(pInfo, nField);
}
void    SetValue(FILE_INFO* pInfo, FIELDTYPE nField, LPCTSTR szValue) {
    STEPSetValue(pInfo, nField, szValue);
}
bool    isModify(FILE_INFO* pInfo) {
    return STEPGetBooleanValue(pInfo,  0);
}
void    SetModifyFlag(FILE_INFO* pInfo, bool bModify) {
    STEPSetBooleanValue(pInfo, 0, bModify);
}
bool    isChecked(FILE_INFO* pInfo) {
    return STEPGetBooleanValue(pInfo,  1);
}
void    SetCheckFlag(FILE_INFO* pInfo, bool bCheck) {
    STEPSetBooleanValue(pInfo, 1, bCheck);
}
bool    isExistLyricFile(FILE_INFO* pInfo) {
    return STEPGetBooleanValue(pInfo,  2);
}
void    SetExistLyricFile(FILE_INFO* pInfo, bool bExist) {
    STEPSetBooleanValue(pInfo, 2, bExist);
}
long    GetFileSize(FILE_INFO* pInfo) {
    return STEPGetLongValue(pInfo, 0);
}
void    SetFileSize(FILE_INFO* pInfo, long size) {
    STEPSetLongValue(pInfo, 0, size);
}
UINT    GetFormat(FILE_INFO* pInfo) {
    return (UINT)STEPGetIntValue(pInfo, 0);
}
void    SetFormat(FILE_INFO* pInfo, UINT nFormat) {
    STEPSetIntValue(pInfo, 0, nFormat);
}
int     GetPlayTime(FILE_INFO* pInfo) {
    return (UINT)STEPGetIntValue(pInfo, 1);
}
void    SetPlayTime(FILE_INFO* pInfo, int nPlayTime) {
    STEPSetIntValue(pInfo, 1, nPlayTime);
}
BYTE    GetBTrackNumber(FILE_INFO* pInfo) {
    return (BYTE)STEPGetIntValue(pInfo, 2);
}
void    SetBTrackNumber(FILE_INFO* pInfo, BYTE nTrackNumber) {
    STEPSetIntValue(pInfo, 2, nTrackNumber);
}
BYTE    GetBGenre(FILE_INFO* pInfo) {
    return (BYTE)STEPGetIntValue(pInfo, 3);
}
void    SetBGenre(FILE_INFO* pInfo, BYTE nGenre) {
    STEPSetIntValue(pInfo, 3, nGenre);
}
void*    GetFileSpecificInfo(FILE_INFO* pInfo) {
    return STEPGetVoidValue(pInfo, 0);
}
void    SetFileSpecificInfo(FILE_INFO* pInfo, void* info) {
    STEPSetVoidValue(pInfo, 0, info);
}

inline LPCTSTR GetFullPath(FILE_INFO* info) {
    return GetValue(info, FIELD_FULL_PATH_NAME);
}
inline LPCTSTR GetFileName(FILE_INFO* info) {
    return GetValue(info, FIELD_FILE_NAME);
}
inline LPCTSTR GetFilePath(FILE_INFO* info) {
    return GetValue(info, FIELD_FILE_PATH);
}
inline LPCTSTR GetVisualFormat(FILE_INFO* info) {
    return GetValue(info, FIELD_VISUAL_FORMAT);
}
inline LPCTSTR GetAudioFormat(FILE_INFO* info) {
    return GetValue(info, FIELD_AUDIO_FORMAT);
}
inline LPCTSTR GetTrackName(FILE_INFO* info) {
    return GetValue(info, FIELD_TRACK_NAME);
}
inline LPCTSTR GetArtistName(FILE_INFO* info) {
    return GetValue(info, FIELD_ARTIST_NAME);
}
inline LPCTSTR GetAlbumName(FILE_INFO* info) {
    return GetValue(info, FIELD_ALBUM_NAME);
}
inline LPCTSTR GetComment(FILE_INFO* info) {
    return GetValue(info, FIELD_COMMENT);
}
inline LPCTSTR GetYear(FILE_INFO* info) {
    return GetValue(info, FIELD_YEAR);
}
inline LPCTSTR GetTrackNumber(FILE_INFO* info) {
    return GetValue(info, FIELD_TRACK_NUMBER);
}
inline LPCTSTR GetGenre(FILE_INFO* info) {
    return GetValue(info, FIELD_GENRE);
}
inline LPCTSTR GetTrackNameSI(FILE_INFO* info) {
    return GetValue(info, FIELD_TRACK_NAME_SI);
}
inline LPCTSTR GetArtistNameSI(FILE_INFO* info) {
    return GetValue(info, FIELD_ARTIST_NAME_SI);
}
inline LPCTSTR GetAlbumNameSI(FILE_INFO* info) {
    return GetValue(info, FIELD_ALBUM_NAME_SI);
}
inline LPCTSTR GetCommentSI(FILE_INFO* info) {
    return GetValue(info, FIELD_COMMENT_SI);
}
inline LPCTSTR GetYearSI(FILE_INFO* info) {
    return GetValue(info, FIELD_YEAR_SI);
}
inline LPCTSTR GetTrackNumberSI(FILE_INFO* info) {
    return GetValue(info, FIELD_TRACK_NUMBER_SI);
}
inline LPCTSTR GetDiscNumberSI(FILE_INFO* info) {
    return GetValue(info, FIELD_DISC_NUMBER_SI); /* STEP J-h FIELD_Disc_NUMBER_SI -> FIELD_DISC_NUMBER_SI */
}
inline LPCTSTR GetTrackTotalSI(FILE_INFO* info) {//by Kobarin
    return GetValue(info, FIELD_TRACK_TOTAL_SI);
}
inline LPCTSTR GetDiscTotalSI(FILE_INFO* info) {//by Kobarin
    return GetValue(info, FIELD_DISC_TOTAL_SI);
}
inline LPCTSTR GetGenreSI(FILE_INFO* info) {
    return GetValue(info, FIELD_GENRE_SI);
}
inline LPCTSTR GetCopyrightSI(FILE_INFO* info) {
    return GetValue(info, FIELD_COPYRIGHT_SI);
}
inline LPCTSTR GetEngineerSI(FILE_INFO* info) {
    return GetValue(info, FIELD_ENGINEER_SI);
}
inline LPCTSTR GetSourceSI(FILE_INFO* info) {
    return GetValue(info, FIELD_SOURCE_SI);
}
inline LPCTSTR GetSoftwareSI(FILE_INFO* info) {
    return GetValue(info, FIELD_SOFTWARE_SI);
}
inline LPCTSTR GetKeywordSI(FILE_INFO* info) {
    return GetValue(info, FIELD_KEYWORD_SI);
}
inline LPCTSTR GetTechnicianSI(FILE_INFO* info) {
    return GetValue(info, FIELD_TECHNICIAN_SI);
}
inline LPCTSTR GetLyricSI(FILE_INFO* info) {
    return GetValue(info, FIELD_LYRIC_SI);
}
inline LPCTSTR GetCommissionSI(FILE_INFO* info) {
    return GetValue(info, FIELD_COMMISSION_SI);
}
inline LPCTSTR GetWriterSI(FILE_INFO* info) {
    return GetValue(info, FIELD_WRITER_SI);
}
inline LPCTSTR GetComposerSI(FILE_INFO* info) {
    return GetValue(info, FIELD_COMPOSER_SI);
}
inline LPCTSTR GetAlbumArtistSI(FILE_INFO* info) {
    return GetValue(info, FIELD_ALBM_ARTIST_SI);
}
inline LPCTSTR GetOrigArtistSI(FILE_INFO* info) {
    return GetValue(info, FIELD_ORIG_ARTIST_SI);
}
inline LPCTSTR GetURLSI(FILE_INFO* info) {
    return GetValue(info, FIELD_URL_SI);
}
inline LPCTSTR GetEncodest(FILE_INFO* info) {
    return GetValue(info, FIELD_ENCODEST);
}
inline LPCTSTR GetOther(FILE_INFO* info) {
    return GetValue(info, FIELD_OTHER);
}
inline LPCTSTR GetFileTypeName(FILE_INFO* info) {
    return GetValue(info, FILED_FILE_TYPE_NAME);
}
//inline LPCTSTR GetAlbumArtistSI(FILE_INFO* info) { /* STEP 042 */
//    return GetValue(info, FILED_ALBUM_ARTIST_SI);
//}
//inline LPCTSTR GetWriterSI(FILE_INFO* info) { /* STEP 043 */
//    return GetValue(info, FILED_WRITER_SI);
//}
inline LPCTSTR GetDiskNumberSI(FILE_INFO* info) { /* STEP 045 */
    return GetValue(info, FIELD_DISK_NUMBER_SI);
}
inline LPCTSTR GetCompilationSI(FILE_INFO* info) { /* STEP 049 */
    return GetValue(info, FIELD_COMPILATION_SI);
}

inline void SetFullPathName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_FULL_PATH_NAME, szValue);
}
inline void SetFileName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_FILE_NAME, szValue);
}
inline void SetFilePath(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_FILE_PATH, szValue);
}
inline void SetVisualFormat(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_VISUAL_FORMAT, szValue);
}
inline void SetAudioFormat(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_AUDIO_FORMAT, szValue);
}
inline void SetTrackName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_TRACK_NAME, szValue);
}
inline void SetArtistName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ARTIST_NAME, szValue);
}
inline void SetAlbumName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ALBUM_NAME, szValue);
}
inline void SetComment(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_COMMENT, szValue);
}
inline void SetYear(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_YEAR, szValue);
}
inline void SetTrackNumber(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_TRACK_NUMBER, szValue);
}
inline void SetGenre(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_GENRE, szValue);
}
inline void SetTrackNameSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_TRACK_NAME_SI, szValue);
}
inline void SetArtistNameSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ARTIST_NAME_SI, szValue);
}
inline void SetAlbumNameSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ALBUM_NAME_SI, szValue);
}
inline void SetCommentSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_COMMENT_SI, szValue);
}
inline void SetYearSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_YEAR_SI, szValue);
}
inline void SetTrackNumberSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_TRACK_NUMBER_SI, szValue);
}
inline void SetDiscNumberSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_DISC_NUMBER_SI, szValue); /* STEP J-h FIELD_Disc_NUMBER_SI -> FIELD_DISC_NUMBER_SI */
}
inline void SetTrackTotalSI(FILE_INFO* info, LPCTSTR szValue) {//by Kobarin
    SetValue(info, FIELD_TRACK_TOTAL_SI, szValue);
}
inline void SetDiscTotalSI(FILE_INFO* info, LPCTSTR szValue) {//by Kobarin
    SetValue(info, FIELD_DISC_TOTAL_SI, szValue);
}
inline void SetGenreSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_GENRE_SI, szValue);
}
inline void SetCopyrightSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_COPYRIGHT_SI, szValue);
}
inline void SetEngineerSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ENGINEER_SI, szValue);
}
inline void SetSourceSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_SOURCE_SI, szValue);
}
inline void SetSoftwareSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_SOFTWARE_SI, szValue);
}
inline void SetKeywordSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_KEYWORD_SI, szValue);
}
inline void SetTechnicianSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_TECHNICIAN_SI, szValue);
}
inline void SetLyricSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_LYRIC_SI, szValue);
}
inline void SetCommissionSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_COMMISSION_SI, szValue);
}
inline void SetWriterSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_WRITER_SI, szValue);
}
inline void SetComposerSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_COMPOSER_SI, szValue);
}
inline void SetAlbumArtistSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ALBM_ARTIST_SI, szValue);
}
inline void SetOrigArtistSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ORIG_ARTIST_SI, szValue);
}
inline void SetURLSI(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_URL_SI, szValue);
}
inline void SetEncodest(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_ENCODEST, szValue);
}
inline void SetOther(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FIELD_OTHER, szValue);
}
inline void SetFileTypeName(FILE_INFO* info, LPCTSTR szValue) {
    SetValue(info, FILED_FILE_TYPE_NAME, szValue);
}
//inline void SetAlbumArtistSI(FILE_INFO* info, LPCTSTR szValue) { /* STEP 042 */
//    SetValue(info, FILED_ALBUM_ARTIST_SI, szValue);
//}
//inline void
// (FILE_INFO* info, LPCTSTR szValue) { /* STEP 043 */
//    SetValue(info, FILED_WRITER_SI, szValue);
//}
inline void SetDiskNumberSI(FILE_INFO* info, LPCTSTR szValue) { /* STEP 045 */
    SetValue(info, FIELD_DISK_NUMBER_SI, szValue);
}
inline void SetCompilationSI(FILE_INFO* info, LPCTSTR szValue) { /* STEP 049 */
    SetValue(info, FIELD_COMPILATION_SI, szValue);
}

// SuperTagEditor.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include <dos.h> //コマンドラインの解析がめんどいので(^^;
#include "INI/ini.h"
#include "SuperTagEditor.h"
#include "AutoBuildCount.h"
#include "MainFrm.h"
#include "SuperTagEditorDoc.h"
#include "SuperTagEditorView.h"
#include "DlgCommonProg.h"

#include "FileMP3.h" /* WildCherry2 078 */

#include "Plugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RECT        g_rectMainWindow;       // メインウィンドウの座標
BOOL        g_bMainFrameZoomed;
BOOL        g_bMainFrameIconic;
CString     g_strCurrentDirectory;
CString     g_strCurrentPlayList;
CString     g_strCurrentMoveDirectory;
bool        g_bIsVersionUp;
int         g_nUserConvFormatType;
USER_CONV_FORMAT    g_userConvFormat[USER_CONV_FORMAT_MAX];
USER_CONV_FORMAT_EX g_userConvFormatEx[USER_CONV_FORMAT_EX_MAX];
USER_MOVE_FODLER_FORMAT g_userMoveFolder[USER_MOVE_FODLER_FORMAT_MAX];
USER_COPY_FORMAT_FORMAT g_userCopyFormat[USER_COPY_FORMAT_FORMAT_MAX]; /* FunnyCorn 175 */
USER_CONV_FORMAT_TAG2TAG g_userConvFormatTag2Tag[USER_CONV_FORMAT_TAG2TAG_MAX]; /* STEP 034 */
TEIKEI_INFO g_teikeiInfo[TEIKEI_INFO_MAX]; /* STEP 035 */
WRITE_FORMAT        g_writeFormat[WRITE_FORMAT_MAX];
bool        g_bOptESCEditCancel;
bool        g_bOptEnableEditCursorExit;
bool        g_bOptEnterBeginEdit;
bool        g_bOptEditOkDown;
bool        g_bOptKeepTimeStamp;
bool        g_bOptChangeFileExt;
bool        g_bOptSyncCreateTime;
bool        g_bOptChangeTextFile;
int         g_nOptCheckFileName;
CString     g_sOptWinAmpPath;
int         g_nOptPlayerType;
bool        g_bOptEditFieldSIF;
bool        g_bOptAutoOpenFolder;
bool        g_bOptLoadFileChecked;
bool        g_bOptHideMP3ListFile;
bool        g_bOptDropSearchSubFolder; /* TyphoonSwell 026 */
bool        g_bOptShowZenSpace; /* BeachMonster 107 */
CString     g_sOptShowOtherChar; /* BeachMonster 107 */ // とりあえずいれとく
bool        g_bOptSortIgnoreCase; /* BeachMonster4 114 */
bool        g_bOptSortIgnoreZenHan; /* BeachMonster4 114 */
bool        g_bOptSortIgnoreKataHira; /* FunnyCorn 179 */
bool        g_bOptShowTotalParent; /* RockDance 128 */
bool        g_bOptShowTips; /* Rumble 188 */
bool        g_bOptLoadFileAdjustColumn;
bool        g_bOptSearchLyricFile;
bool        g_bOptSetLyricsDir;
bool        g_bOptSearchLyricsSubDir;
bool        g_bEnableSearchSubDir;
bool        g_bEnableMoveFolderCopy;
CString     g_strOptLyricsPath;
SORT_STATE  g_sortState[SORT_KEY_MAX];
CLASS_INFO  g_classInfo;
CHECK_WORD_STATE    g_chkWord[CHECK_STATE_MAX];

//bool      g_bOptID3v2GenreAddNumber;
bool        g_bConfFileNameMaxCheck;
bool        g_bFileNameMaxCellColor; /* SeaKnows 036 */
int         g_nConfFileNameMaxChar;

bool        g_bEnableFolderSync;
CString     g_strRootFolder;
bool        g_bSyncSelectAlways;
bool        g_bSyncDeleteFolder;
bool        g_bSyncLyricsFileMove;

bool        g_bConfDeleteFile;
bool        g_bConfDeleteList;
bool        g_bConfEditModify;
bool        g_bConfFolderSync;

bool        g_bPlayListClearList;
bool        g_bPlayListClearCheck;
bool        g_bPlayListAddList;
bool        g_bPlayListFileCheck;

/* STEP 035 *///CString  g_strTeikei[10+20]; /* SeaKnows 030 *//* FreeFall 046 */
CString     g_strTeikeiGroupName[3];    /* FreeFall 046 */
bool        g_bValidFolderSelect;   /* SeaKnows 033 */
bool        g_bValidDupExec;    /* FreeFall 045 */

int         g_nRecentFolder; /* StartInaction 053 */
bool        g_bSaveRepDlgPos; /* WildCherry4 086 */
int         g_nSaveRepDlgPosX; /* WildCherry4 086 */
int         g_nSaveRepDlgPosY; /* WildCherry4 086 */

// 文字種統一 /* StartInaction 054 */
UINT        g_nUnifyAlpha;
UINT        g_nUnifyHiraKata;
UINT        g_nUnifyKata;
UINT        g_nUnifyKigou;
UINT        g_nUnifySuji;
UINT        g_nUnifyUpLow;
UINT        g_nUnifyFixedUpLow; /* STEP 040 */

// ファイル名文字種統一 /* LastTrain 058 */
UINT        g_nFileUnifyAlpha;
UINT        g_nFileUnifyHiraKata;
UINT        g_nFileUnifyKata;
UINT        g_nFileUnifyKigou;
UINT        g_nFileUnifySuji;
UINT        g_nFileUnifyUpLow;

// 拡張子変換 /* STEP 006 */
UINT        g_nFileExtChange;

bool        g_bEndEditMoveRightCell; /* BeachMonster 091 */

bool        g_bShowLoadPlaylistDlg; /* RockDance 126 */

CString     g_strFavorite[10];  /* RockDance 129 */

int         g_nAddNumberWidth; /* Baja 159 */
int         g_nAddNumberPos; /* Baja 159 */
CString     g_strAddNumberSep; /* Baja 159 */
CString     g_strAddNumberBef; /* Conspiracy 194 */
CString     g_strAddNumberAft; /* Conspiracy 194 */

bool        g_bAudioListShow; /* Conspiracy 199 */

bool        g_bFirstUpperIgnoreWord; /* STEP 026 */
CString     g_strFirstUpperIgnoreWords; /* STEP 026 */
CString     g_strFirstUpperSentenceSeparator; /* STEP 026 */
bool        g_bUserConvAddMenu; /* STEP 030 */
bool        g_bZenHanKigouKana; /* STEP 016 */

bool        g_bAutoTilde2WaveDash;//STEP_K (全角チルダを波ダッシュに自動置換）

LOGFONT     g_fontReport;           // レポートウィンドウのフォント

FILENAME_REPLACE    g_fileNameReplace[FILENAME_REPLACE_MAX];

FILENAME_REPLACE    g_userFileNameReplace[USER_FILENAME_REPLACE_MAX];   /* FreeFall 050 */

const TCHAR *g_sRepTable[FILENAME_REPLACE_MAX][2] = {
    {_T("\""), _T("”")}, {_T("*") , _T("＊")}, {_T(",") , _T("，")}, {_T("/") , _T("／")},
    {_T(":") , _T("：")}, {_T(";") , _T("；")}, {_T("<") , _T("＜")}, {_T(">") , _T("＞")},
    {_T("?") , _T("？")}, {_T("\\"), _T("￥")}, {_T("|") , _T("｜")}, {_T(" ") , _T(" ")},
};

CStringArray g_arFixedWords; /* STEP 040 */

#define HK_CTRL_C               0x00020043
#define HK_CTRL_D               0x00020044
#define HK_CTRL_F               0x00020046
#define HK_CTRL_H               0x00020048
#define HK_CTRL_N               0x0002004e
#define HK_CTRL_O               0x0002004f
#define HK_CTRL_S               0x00020053
#define HK_CTRL_V               0x00020056
#define HK_CTRL_X               0x00020058

KEY_CONFIG  g_listKeyConfig[] = {
    /**
     【注意】コマンドを追加した場合は、_APS_NEXT_COMMAND_VALUEが更新されるのでSTEP_api.cppをリコンパイルすること
     **/
    // ファイル処理
    {ID_FILE_NEW                  , 0x00000000, COMMAND_GROUP_FILE, _T("リストをクリア"), _T("FileNew")},
    {ID_OPEN_FOLDER               , HK_CTRL_O , COMMAND_GROUP_FILE, _T("フォルダを開く"), _T("FileOpenFolder")},
    {ID_SAVE_ALL_TAG              , HK_CTRL_S , COMMAND_GROUP_FILE, _T("タグ情報を更新"), _T("FileSaveAllTag")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_FILE, _T("--------------------"), NULL},
    {ID_WRITE_LIST1               , 0x00000000, COMMAND_GROUP_FILE, _T("リスト出力：書式１"), _T("WriteList1")},
    {ID_WRITE_LIST2               , 0x00000000, COMMAND_GROUP_FILE, _T("リスト出力：書式２"), _T("WriteList2")},
    {ID_WRITE_LIST3               , 0x00000000, COMMAND_GROUP_FILE, _T("リスト出力：書式３"), _T("WriteList3")},
    {ID_WRITE_LIST4               , 0x00000000, COMMAND_GROUP_FILE, _T("リスト出力：書式４"), _T("WriteList4")},
    {ID_WRITE_LIST5               , 0x00000000, COMMAND_GROUP_FILE, _T("リスト出力：書式５"), _T("WriteList5")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_FILE, _T("--------------------"), NULL},
    {ID_MOVE_FOLDER_01            , 0x00000000, COMMAND_GROUP_FILE, _T("ファイル移動：書式１"), _T("MoveFolder1")},
    {ID_MOVE_FOLDER_02            , 0x00000000, COMMAND_GROUP_FILE, _T("ファイル移動：書式２"), _T("MoveFolder2")},
    {ID_MOVE_FOLDER_03            , 0x00000000, COMMAND_GROUP_FILE, _T("ファイル移動：書式３"), _T("MoveFolder3")},
    {ID_MOVE_FOLDER_04            , 0x00000000, COMMAND_GROUP_FILE, _T("ファイル移動：書式４"), _T("MoveFolder4")},
    {ID_MOVE_FOLDER_05            , 0x00000000, COMMAND_GROUP_FILE, _T("ファイル移動：書式５"), _T("MoveFolder5")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_FILE, _T("--------------------"), NULL},
    {ID_FAVORITE_FOLDER_01        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り１"), _T("FavoriteFolder1")},
    {ID_FAVORITE_FOLDER_02        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り２"), _T("FavoriteFolder2")},
    {ID_FAVORITE_FOLDER_03        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り３"), _T("FavoriteFolder3")},
    {ID_FAVORITE_FOLDER_04        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り４"), _T("FavoriteFolder4")},
    {ID_FAVORITE_FOLDER_05        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り５"), _T("FavoriteFolder5")},
    {ID_FAVORITE_FOLDER_06        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り６"), _T("FavoriteFolder6")},
    {ID_FAVORITE_FOLDER_07        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り７"), _T("FavoriteFolder7")},
    {ID_FAVORITE_FOLDER_08        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り８"), _T("FavoriteFolder8")},
    {ID_FAVORITE_FOLDER_09        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り９"), _T("FavoriteFolder9")},
    {ID_FAVORITE_FOLDER_10        , 0x00000000, COMMAND_GROUP_FILE, _T("お気に入りのフォルダ：お気に入り10"), _T("FavoriteFolder10")},

    // 編集処理
    {ID_EDIT_COPY                 , HK_CTRL_C , COMMAND_GROUP_EDIT, _T("コピー")  , _T("EditCopy")},
    {ID_EDIT_PASTE                , HK_CTRL_V , COMMAND_GROUP_EDIT, _T("貼り付け"), _T("EditPaste")},
    {ID_EDIT_CUT                  , HK_CTRL_X , COMMAND_GROUP_EDIT, _T("切り取り"), _T("EditCut")},
    {ID_EDIT_PASTE_ADD            , 0x00000000, COMMAND_GROUP_EDIT, _T("追加で貼り付け"), _T("EditPasteAdd")}, /* FunnyCorn 176 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("----- 定型文 グループ１ -----"), NULL},
    {ID_TEIKEI_01                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-１"), _T("TeikeiPaste1-1")}, /* SeaKnows 030 */
    {ID_TEIKEI_02                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-２"), _T("TeikeiPaste1-2")}, /* SeaKnows 030 */
    {ID_TEIKEI_03                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-３"), _T("TeikeiPaste1-3")}, /* SeaKnows 030 */
    {ID_TEIKEI_04                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-４"), _T("TeikeiPaste1-4")}, /* SeaKnows 030 */
    {ID_TEIKEI_05                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-５"), _T("TeikeiPaste1-5")}, /* SeaKnows 030 */
    {ID_TEIKEI_06                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-６"), _T("TeikeiPaste1-6"}), /* SeaKnows 030 */
    {ID_TEIKEI_07                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-７"), _T("TeikeiPaste1-7")}, /* SeaKnows 030 */
    {ID_TEIKEI_08                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-８"), _T("TeikeiPaste1-8")}, /* SeaKnows 030 */
    {ID_TEIKEI_09                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-９"), _T("TeikeiPaste1-9")}, /* SeaKnows 030 */
    {ID_TEIKEI_10                 , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け１-１０"), _T("TeikeiPaste1-10")}, /* SeaKnows 030 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("----- 定型文 グループ２ -----"), NULL},
    {ID_TEIKEI_2_01               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-１"), _T("TeikeiPaste2-1")}, /* FreeFall 046 */
    {ID_TEIKEI_2_02               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-２"), _T("TeikeiPaste2-2")}, /* FreeFall 046 */
    {ID_TEIKEI_2_03               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-３"), _T("TeikeiPaste2-3")}, /* FreeFall 046 */
    {ID_TEIKEI_2_04               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-４"), _T("TeikeiPaste2-4")}, /* FreeFall 046 */
    {ID_TEIKEI_2_05               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-５"), _T("TeikeiPaste2-5")}, /* FreeFall 046 */
    {ID_TEIKEI_2_06               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-６"), _T("TeikeiPaste2-6")}, /* FreeFall 046 */
    {ID_TEIKEI_2_07               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-７"), _T("TeikeiPaste2-7")}, /* FreeFall 046 */
    {ID_TEIKEI_2_08               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-８"), _T("TeikeiPaste2-8")}, /* FreeFall 046 */
    {ID_TEIKEI_2_09               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-９"), _T("TeikeiPaste2-9")}, /* FreeFall 046 */
    {ID_TEIKEI_2_10               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け２-１０"), _T("TeikeiPaste2-10")}, /* FreeFall 046 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("----- 定型文 グループ３ -----"), NULL},
    {ID_TEIKEI_3_01               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-１"), _T("TeikeiPaste3-1")}, /* FreeFall 046 */
    {ID_TEIKEI_3_02               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-２"), _T("TeikeiPaste3-2")}, /* FreeFall 046 */
    {ID_TEIKEI_3_03               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-３"), _T("TeikeiPaste3-3")}, /* FreeFall 046 */
    {ID_TEIKEI_3_04               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-４"), _T("TeikeiPaste3-4")}, /* FreeFall 046 */
    {ID_TEIKEI_3_05               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-５"), _T("TeikeiPaste3-5")}, /* FreeFall 046 */
    {ID_TEIKEI_3_06               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-６"), _T("TeikeiPaste3-6")}, /* FreeFall 046 */
    {ID_TEIKEI_3_07               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-７"), _T("TeikeiPaste3-7")}, /* FreeFall 046 */
    {ID_TEIKEI_3_08               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-８"), _T("TeikeiPaste3-8")}, /* FreeFall 046 */
    {ID_TEIKEI_3_09               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-９"), _T("TeikeiPaste3-9")}, /* FreeFall 046 */
    {ID_TEIKEI_3_10               , 0x00000000, COMMAND_GROUP_EDIT, _T("定型文貼り付け３-１０"), _T("TeikeiPaste3-10")}, /* FreeFall 046 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_FILE, _T("--------------------"), NULL},
    {ID_EDIT_COPY_FORMAT_01       , 0x00000000, COMMAND_GROUP_FILE, _T("書式コピー：書式１"), _T("CopyFormat1")},
    {ID_EDIT_COPY_FORMAT_02       , 0x00000000, COMMAND_GROUP_FILE, _T("書式コピー：書式２"), _T("CopyFormat2")},
    {ID_EDIT_COPY_FORMAT_03       , 0x00000000, COMMAND_GROUP_FILE, _T("書式コピー：書式３"), _T("CopyFormat3")},
    {ID_EDIT_COPY_FORMAT_04       , 0x00000000, COMMAND_GROUP_FILE, _T("書式コピー：書式４"), _T("CopyFormat4")},
    {ID_EDIT_COPY_FORMAT_05       , 0x00000000, COMMAND_GROUP_FILE, _T("書式コピー：書式５"), _T("CopyFormat5")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("--------------------"), NULL},
    {ID_CELL_COPY_DOWN            , HK_CTRL_D , COMMAND_GROUP_EDIT, _T("下方向へコピー"), _T("EditCopyDown")},
    {ID_SET_NUMBER                , HK_CTRL_N , COMMAND_GROUP_EDIT, _T("下方向へ連番")  , _T("EditSetNumber")},
    {ID_SET_NUMBER_ADD            , 0x00000000, COMMAND_GROUP_EDIT, _T("下方向へ連番（先頭に追加）")  , _T("EditSetNumberAdd")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("--------------------"), NULL},
    {ID_EDIT_FIND                 , 0x00000000, COMMAND_GROUP_EDIT, _T("検索"), _T("EditFind")},
    {ID_EDIT_REPLACE              , 0x00000000, COMMAND_GROUP_EDIT, _T("置換"), _T("EditReplace")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("--------------------"), NULL},
    {ID_SELECT_DELETE_LIST        , 0x00000000, COMMAND_GROUP_EDIT, _T("STEのリストから削除")         , _T("DeleteList")},
    {ID_SELECT_DELETE_FILE        , 0x00000000, COMMAND_GROUP_EDIT, _T("ファイルの削除")              , _T("DeleteFile")},
    {ID_SELECT_EDIT_DESTORY       , 0x00000000, COMMAND_GROUP_EDIT, _T("変更前の状態に戻す")          , _T("EditModify")},
    {ID_FOLDER_TREE_SYNC          , 0x00000000, COMMAND_GROUP_EDIT, _T("フォルダ構成の同期")          , _T("FolderSync")},
    {ID_CHECK_FILE_SYNC           , 0x00000000, COMMAND_GROUP_EDIT, _T("フォルダ構成の同期(チェックのみ)"), _T("CheckFileSync")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_EDIT, _T("--------------------"), NULL},
    {ID_DELETE_CHAR_SPACE         , 0x00000000, COMMAND_GROUP_EDIT, _T("先頭/末尾の空白文字を削除")   , _T("EditDeleteCharSpace")}, /* Rumble 192 */
    {ID_DELETE_CHAR               , 0x00000000, COMMAND_GROUP_EDIT, _T("先頭/末尾のｎ文字を削除")     , _T("EditDeleteChar")},

    // 表示
    {ID_ADJUST_COLUMN_WIDTH       , 0x00000000, COMMAND_GROUP_DISP, _T("全てのカラム幅を調整"), _T("AdjustColumnWidth")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_DISP, _T("--------------------"), NULL},
    {ID_SORT_LIST                 , 0x00000000, COMMAND_GROUP_DISP, _T("並び替え")        , _T("ListSort")},
    {ID_SORT_LIST_DIRECT          , 0x00000000, COMMAND_GROUP_DISP, _T("並び替えを再実行"), _T("ListSortDirect")},
    {ID_EXEC_CLASSIFICATION       , 0x00000000, COMMAND_GROUP_DISP, _T("分類表示を更新")  , _T("ExecClassification")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_DISP, _T("--------------------"), NULL},
    {ID_EDIT_FIELD_SI             , 0x00000000, COMMAND_GROUP_DISP, _T("SIフィールドを表示/編集")       , _T("EditFieldSI")},
    {ID_EDIT_TD3_TAG              , 0x00000000, COMMAND_GROUP_DISP, _T("ID3 tag を表示/編集")           , _T("EditID3tag")},
    {ID_EDIT_CHANGE_FIELD         , 0x00000000, COMMAND_GROUP_DISP, _T("[ID3 tag]<=>[SIフィールド]切替"), _T("EditChangeField")},

    // プレイリスト
    {ID_LOAD_PLAYLIST             , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("プレイリストの読み込み")        , _T("LoadPlayList")},
    {ID_WRITE_PLAYLIST            , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("プレイリストの出力")            , _T("WritePlayList")},
    {ID_WRITE_TREE_PLAYLIST       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("ツリー分類のプレイリスト出力")  , _T("WriteTreePlayList")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("--------------------"), NULL},
    {ID_ALL_FILES_CHECK           , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("全てのファイルをチェックする")  , _T("AllFilesCheck")},
    {ID_ALL_FILES_UNCHECK         , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("全てのファイルのチェックを外す"), _T("AllFilesUnCheck")},
    {ID_REVERSE_CHECK             , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("チェック状態を反転する")        , _T("ReverseCheck")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("--------------------"), NULL},
    {ID_CHECK_FILES_SELECT        , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("チェックファイルを選択")        , _T("CheckFilesSelect")},
    {ID_SELECT_FILES_CHECK        , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("選択ファイルをチェック")        , _T("SelectFilesCheck")},
    {ID_SELECT_FILES_UNCHECK      , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("選択ファイルのチェックを外す")  , _T("SelectFilesUnCheck")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("--------------------"), NULL},
    {ID_CHECK_WORD                , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("条件を指定してチェック")        , _T("CheckWord")},
    {ID_CHECK_FILENAME_MAX        , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("ファイル名の長さをチェック")    , _T("CheckFileNameMax")}, /* SeaKnows 037 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("--------------------"), NULL},
    {ID_SELECT_TREE_COLUM         , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("ツリー配下のセル一括選択")    , _T("SelectTreeColumn")}, /* TyphoonSwell 025 */
    {ID_SELECT_TREE_FILE          , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("ツリー配下のファイル一括選択")  , _T("SelectTreeFile")}, /* STEP 013 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("--------------------"), NULL},
    {ID_MOVE_TO_PARENT            , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("カーソルを親ツリーへ移動")      , _T("MoveToParent")}, /* STEP 014 */
    {ID_MOVE_TO_PREVIOUS          , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("カーソル位置を一つ上のツリーへ移動")      , _T("MoveToPrevious")}, /* STEP 014 */
    {ID_MOVE_TO_NEXT              , 0x00000000, COMMAND_GROUP_PLAYLIST, _T("カーソル位置を一つ下のツリーへ移動")      , _T("MoveToNext")}, /* STEP 014 */

    // WinAmp 制御処理
    {ID_WINAMP_PLAY               , 0x00000000, COMMAND_GROUP_PLAYER, _T("再生")                  , _T("WinampPlay")},
    {ID_WINAMP_STOP               , 0x00000000, COMMAND_GROUP_PLAYER, _T("停止(Winampのみ有効)")  , _T("WinampStop")},
    {ID_WINAMP_EXIT               , 0x00000000, COMMAND_GROUP_PLAYER, _T("終了")                  , _T("WinampExit")},
    {ID_WINAMP_PLAY_PREV          , 0x00000000, COMMAND_GROUP_PLAYER, _T("前の曲(Winampのみ有効)"), _T("WinampPlayPrev")},
    {ID_WINAMP_PLAY_NEXT          , 0x00000000, COMMAND_GROUP_PLAYER, _T("次の曲(Winampのみ有効)"), _T("WinampPlayNext")},

    // 変換処理
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- デフォルト書式変換 -----"), NULL},
    {ID_CONV_FILENAME_TO_TRACKNAME, 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => トラック名"), _T("ConvFileNameToTrackName")},
    {ID_CONV_TRACKNAME_TO_FILENAME, 0x00000000, COMMAND_GROUP_CONV, _T("トラック名 => ファイル名"), _T("ConvTrackNameToFileName")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- ユーザー指定書式変換 -----"), NULL},
    {ID_CONV_TAG2FILE_USER        , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名"), _T("ConvUserTagToFile")},
    {ID_CONV_FILE2TAG_USER        , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報"), _T("ConvUserFileToTag")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("------ 文字変換 -----"), NULL},
    {ID_CONV_STR_HAN_ALL          , 0x00000000, COMMAND_GROUP_CONV, _T("全角=>半角(全て)変換")          , _T("ConvHanAll")},
    {ID_CONV_STR_HAN_KIGOU        , 0x00000000, COMMAND_GROUP_CONV, _T("全角=>半角(記号)変換")          , _T("ConvHanKigou")},
    {ID_CONV_STR_HAN_SUJI         , 0x00000000, COMMAND_GROUP_CONV, _T("全角=>半角(数字)変換")          , _T("ConvHanSuji")},
    {ID_CONV_STR_HAN_KATA         , 0x00000000, COMMAND_GROUP_CONV, _T("全角=>半角(カタカナ)変換")      , _T("ConvHanLata")},
    {ID_CONV_STR_HAN_ALPHA        , 0x00000000, COMMAND_GROUP_CONV, _T("全角=>半角(アルファベット)変換"), _T("ConvHanAlpha")},
    {ID_CONV_STR_ZEN_ALL          , 0x00000000, COMMAND_GROUP_CONV, _T("半角=>全角(全て)変換")          , _T("ConvZenAll")},
    {ID_CONV_STR_ZEN_KIGOU        , 0x00000000, COMMAND_GROUP_CONV, _T("半角=>全角(記号)変換")          , _T("ConvZenKigou")},
    {ID_CONV_STR_ZEN_SUJI         , 0x00000000, COMMAND_GROUP_CONV, _T("半角=>全角(数字)変換")          , _T("ConvZenSuji")},
    {ID_CONV_STR_ZEN_KATA         , 0x00000000, COMMAND_GROUP_CONV, _T("半角=>全角(カタカナ)変換")      , _T("ConvZenKata")},
    {ID_CONV_STR_ZEN_ALPHA        , 0x00000000, COMMAND_GROUP_CONV, _T("半角=>全角(アルファベット)変換"), _T("ConvZenAlpha")},
    {ID_CONV_STR_TO_UPPER         , 0x00000000, COMMAND_GROUP_CONV, _T("小文字=>大文字変換")            , _T("ConvToUpper")},
    {ID_CONV_STR_TO_LOWER         , 0x00000000, COMMAND_GROUP_CONV, _T("大文字=>小文字変換")            , _T("ConvToLower")},
    {ID_CONV_STR_FIRST_UPPER      , 0x00000000, COMMAND_GROUP_CONV, _T("単語の１文字目のみ大文字")      , _T("ConvFirstUpper")},
    {ID_CONV_STR_FIXED_UPPER_LOWER, 0x00000000, COMMAND_GROUP_CONV, _T("大文字小文字固定")              , _T("ConvFIxedUpLow")}, /* STEP 040 */
    {ID_CONV_STR_HIRA2KATA        , 0x00000000, COMMAND_GROUP_CONV, _T("ひらがな=>カタカナ変換")        , _T("ConvHira2Kata")},
    {ID_CONV_STR_KATA2HIRA        , 0x00000000, COMMAND_GROUP_CONV, _T("カタカナ=>ひらがな変換")        , _T("ConvKata2Hira")},
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- 拡張書式変換 -----"), NULL},
    {ID_CONV_FORMAT_EX_01         , 0x00000000, COMMAND_GROUP_CONV, _T("書式１"), _T("ConvFormatEx01")},
    {ID_CONV_FORMAT_EX_02         , 0x00000000, COMMAND_GROUP_CONV, _T("書式２"), _T("ConvFormatEx02")},
    {ID_CONV_FORMAT_EX_03         , 0x00000000, COMMAND_GROUP_CONV, _T("書式３"), _T("ConvFormatEx03")},
    {ID_CONV_FORMAT_EX_04         , 0x00000000, COMMAND_GROUP_CONV, _T("書式４"), _T("ConvFormatEx04")},
    {ID_CONV_FORMAT_EX_05         , 0x00000000, COMMAND_GROUP_CONV, _T("書式５"), _T("ConvFormatEx05")},
    {ID_CONV_FORMAT_EX_06         , 0x00000000, COMMAND_GROUP_CONV, _T("書式６"), _T("ConvFormatEx06")},
    {ID_CONV_FORMAT_EX_07         , 0x00000000, COMMAND_GROUP_CONV, _T("書式７"), _T("ConvFormatEx07")},
    {ID_CONV_FORMAT_EX_08         , 0x00000000, COMMAND_GROUP_CONV, _T("書式８"), _T("ConvFormatEx08")},
    {ID_CONV_FORMAT_EX_09         , 0x00000000, COMMAND_GROUP_CONV, _T("書式９"), _T("ConvFormatEx09")},
    {ID_CONV_FORMAT_EX_10         , 0x00000000, COMMAND_GROUP_CONV, _T("書式１０"), _T("ConvFormatEx10")},
    {ID_CONV_EX_SETUP             , 0x00000000, COMMAND_GROUP_CONV, _T("拡張書式変換書式設定"), _T("ConvUserSetup")}, /* STEP 009 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- ユーザー指定書式の切替 -----"), NULL},
    {ID_CONV_FORMAT_USER_01       , 0x00000000, COMMAND_GROUP_CONV, _T("書式１"), _T("ConvFormatUser01")},
    {ID_CONV_FORMAT_USER_02       , 0x00000000, COMMAND_GROUP_CONV, _T("書式２"), _T("ConvFormatUser02")},
    {ID_CONV_FORMAT_USER_03       , 0x00000000, COMMAND_GROUP_CONV, _T("書式３"), _T("ConvFormatUser03")},
    {ID_CONV_FORMAT_USER_04       , 0x00000000, COMMAND_GROUP_CONV, _T("書式４"), _T("ConvFormatUser04")}, /* LastTrain 057 */
    {ID_CONV_FORMAT_USER_05       , 0x00000000, COMMAND_GROUP_CONV, _T("書式５"), _T("ConvFormatUser05")}, /* LastTrain 057 */
    {ID_CONV_USER_SETUP           , 0x00000000, COMMAND_GROUP_CONV, _T("ユーザー指定書式変換書式設定"), _T("ConvUserSetup")}, /* STEP 009 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- ユーザー指定書式 タグ情報 => ファイル名 -----"), NULL},
    {ID_CONV_FORMAT_USER_T2F_01   , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名 書式１"), _T("ConvFormatUserT2F01")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_T2F_02   , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名 書式２"), _T("ConvFormatUserT2F02")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_T2F_03   , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名 書式３"), _T("ConvFormatUserT2F03")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_T2F_04   , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名 書式４"), _T("ConvFormatUserT2F04")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_T2F_05   , 0x00000000, COMMAND_GROUP_CONV, _T("タグ情報 => ファイル名 書式５"), _T("ConvFormatUserT2F05")}, /* STEP 030 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- ユーザー指定書式 ファイル名 => タグ情報 -----"), NULL},
    {ID_CONV_FORMAT_USER_F2T_01   , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報 書式１"), _T("ConvFormatUserF2T01")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_F2T_02   , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報 書式２"), _T("ConvFormatUserF2T02")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_F2T_03   , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報 書式３"), _T("ConvFormatUserF2T03")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_F2T_04   , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報 書式４"), _T("ConvFormatUserF2T04")}, /* STEP 030 */
    {ID_CONV_FORMAT_USER_F2T_05   , 0x00000000, COMMAND_GROUP_CONV, _T("ファイル名 => タグ情報 書式５"), _T("ConvFormatUserF2T05")}, /* STEP 030 */
    {0x0000                       , 0x00000000, COMMAND_GROUP_CONV, _T("----- タグ情報変換 -----"), NULL}, /* STEP 034 */
    {ID_CONV_TAG_TO_TAG_01        , 0x00000000, COMMAND_GROUP_CONV, _T("書式１"), _T("ConvFormatTag2Tag01")}, /* STEP 034 */
    {ID_CONV_TAG_TO_TAG_02        , 0x00000000, COMMAND_GROUP_CONV, _T("書式２"), _T("ConvFormatTag2Tag02")}, /* STEP 034 */
    {ID_CONV_TAG_TO_TAG_03        , 0x00000000, COMMAND_GROUP_CONV, _T("書式３"), _T("ConvFormatTag2Tag03")}, /* STEP 034 */
    {ID_CONV_TAG_TO_TAG_04        , 0x00000000, COMMAND_GROUP_CONV, _T("書式４"), _T("ConvFormatTag2Tag04")}, /* STEP 034 */
    {ID_CONV_TAG_TO_TAG_05        , 0x00000000, COMMAND_GROUP_CONV, _T("書式５"), _T("ConvFormatTag2Tag05")}, /* STEP 034 */

    {0x0000, 0x00000000, -1, NULL, NULL},       // 終端コード
    /**
     【注意】コマンドを追加した場合は、_APS_NEXT_COMMAND_VALUEが更新されるのでSTEP_api.cppをリコンパイルすること
     **/
};

KEY_CONFIG *SearchKeyConfigID(WORD wCmdID)
{
    int     i;
    for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
        KEY_CONFIG  *pKey = &g_listKeyConfig[i];
        if (pKey->wCmdID == wCmdID) {
            return(pKey);
        }
    }
    extern CPlugin plugins;
    for (i=0;i<plugins.arPluginKey.GetSize();i++) {
        KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
        if (pKey->wCmdID == wCmdID) {
            return pKey;
        }
    }
    return(NULL);
}

static TCHAR    *g_sKeyName[] = {
    _T(""),             // 00
    _T("LBUTTON"),      // 01   マウスの左ボタン
    _T("RBUTTON"),      // 02   マウスの右ボタン
    _T("CANCEL"),       // 03   コントロール ブレーク処理に使用
    _T("MBUTTON"),      // 04   マウスの中央ボタン (3つボタンのマウス)
    _T(""),_T(""),_T(""),       // 05～07   未定義
    _T("BS"),           // 08   BackSpaceキー
    _T("Tab"),          // 09   Tabキー
    _T(""),_T(""),          // 0A､ 0B   未定義
    _T("CLEAR"),        // 0C   Clearキー
    _T("Enter"),        // 0D   Enterキー
    _T(""),_T(""),          // 0E､ 0F   未定義
    _T("Shift"),        // 10   Shiftキー
    _T("Ctrl"),         // 11   Ctrlキー
    _T("Alt"),          // 12   Altキー
    _T("PAUSE"),        // 13   Pauseキー
    _T("CAPITAL"),      // 14   Caps Lockキー
    _T("KANA"),         // 15   英数カナキー
    _T(""),_T(""),_T(""),       // 16～18   漢字システム用に予約
    _T("KANJI"),        // 19   漢字システム用に予約
    _T(""),             // 1A   未定義
    _T("ESC"),          // 1B   Escキー
    _T("CONVERT"),      // 1C   漢字システム用に予約
    _T("NOCONVERT"),    // 1D   漢字システム用に予約
    _T(""),_T(""),          // 1E､ 1F   漢字システム用に予約
    _T("Space"),        // 20   Spaceキー
    _T("PageUp"),       // 21   Page Upキー
    _T("PageDown"),     // 22   Page Downキー
    _T("End"),          // 23   Endキー
    _T("Home"),         // 24   Homeキー
    _T("←"),           // 25   ←キー
    _T("↑"),           // 26   ↑キー
    _T("→"),           // 27   →キー
    _T("↓"),           // 28   ↓キー
    _T("SELECT"),       // 29   Selectキー
    _T(""),             // 2A   OEM指定
    _T("EXECUTE"),      // 2B   Executeキー
    _T("SNAPSHOT"),     // 2C   Print Screenキー (Windows 3.0以降用)
    _T("Ins"),          // 2D   Insキー
    _T("Del"),          // 2E   Delキー
    _T("Help"),         // 2F   Helpキー
    _T("0"),            // 30   0キー
    _T("1"),            // 31   1キー
    _T("2"),            // 32   2キー
    _T("3"),            // 33   3キー
    _T("4"),            // 34   4キー
    _T("5"),            // 35   5キー
    _T("6"),            // 36   6キー
    _T("7"),            // 37   7キー
    _T("8"),            // 38   8キー
    _T("9"),            // 39   9キー
    _T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),   // 3A～40   未定義
    _T("A"),            // 41   Aキー
    _T("B"),            // 42   Bキー
    _T("C"),            // 43   Cキー
    _T("D"),            // 44   Dキー
    _T("E"),            // 45   Eキー
    _T("F"),            // 46   Fキー
    _T("G"),            // 47   Gキー
    _T("H"),            // 48   Hキー
    _T("I"),            // 49   Iキー
    _T("J"),            // 4A   Jキー
    _T("K"),            // 4B   Kキー
    _T("L"),            // 4C   Lキー
    _T("M"),            // 4D   Mキー
    _T("N"),            // 4E   Nキー
    _T("O"),            // 4F   Oキー
    _T("P"),            // 50   Pキー
    _T("Q"),            // 51   Qキー
    _T("R"),            // 52   Rキー
    _T("S"),            // 53   Sキー
    _T("T"),            // 54   Tキー
    _T("U"),            // 55   Uキー
    _T("V"),            // 56   Vキー
    _T("W"),            // 57   Wキー
    _T("X"),            // 58   Xキー
    _T("Y"),            // 59   Yキー
    _T("Z"),            // 5A   Zキー
    _T(""),_T(""),_T(""),_T(""),_T(""), // 5B～5F   未定義
    _T("NUM0"),         // 60   テンキーの0キー
    _T("NUM1"),         // 61   テンキーの1キー
    _T("NUM2"),         // 62   テンキーの2キー
    _T("NUM3"),         // 63   テンキーの3キー
    _T("NUM4"),         // 64   テンキーの4キー
    _T("NUM5"),         // 65   テンキーの5キー
    _T("NUM6"),         // 66   テンキーの6キー
    _T("NUM7"),         // 67   テンキーの7キー
    _T("NUM8"),         // 68   テンキーの8キー
    _T("NUM9"),         // 69   テンキーの9キー
    _T("[*]"),          // 6A   テンキーの*キー
    _T("[+]"),          // 6B   テンキーの+キー
    _T("SEPARATOR"),    // 6C   Separatorキー
    _T("[-]"),          // 6D   テンキーの－キー
    _T("[.]"),          // 6E   テンキーの.キー
    _T("[/]"),          // 6F   テンキーの/キー
    _T("F1"),           // 70   F1キー
    _T("F2"),           // 71   F2キー
    _T("F3"),           // 72   F3キー
    _T("F4"),           // 73   F4キー
    _T("F5"),           // 74   F5キー
    _T("F6"),           // 75   F6キー
    _T("F7"),           // 76   F7キー
    _T("F8"),           // 77   F8キー
    _T("F9"),           // 78   F9キー
    _T("F10"),          // 79   F10キー
    _T("F11"),          // 7A   F11キー
    _T("F12"),          // 7B   F12キー
    _T("F13"),          // 7C   F13キー
    _T("F14"),          // 7D   F14キー
    _T("F15"),          // 7E   F15キー
    _T("F16"),          // 7F   F16キー
    _T("F17"),          // 80H  F17キー
    _T("F18"),          // 81H  F18キー
    _T("F19"),          // 82H  F19キー
    _T("F20"),          // 83H  F20キー
    _T("F21"),          // 84H  F21キー
    _T("F22"),          // 85H  F22キー
    _T("F23"),          // 86H  F23キー
    _T("F24"),          // 87H  F24キー
    _T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),    // 88～8F   未定義
    _T("NUMLOCK"),      // 90   Num Lockキー
    _T("SCROLL"),       // 91   Scroll Lockキー
};

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp

BEGIN_MESSAGE_MAP(CSuperTagEditorApp, CWinApp)
    //{{AFX_MSG_MAP(CSuperTagEditorApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateFileMruFile)
    ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_01, OnUpdateFavoriteFolder01)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_02, OnUpdateFavoriteFolder02)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_03, OnUpdateFavoriteFolder03)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_04, OnUpdateFavoriteFolder04)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_05, OnUpdateFavoriteFolder05)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_06, OnUpdateFavoriteFolder06)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_07, OnUpdateFavoriteFolder07)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_08, OnUpdateFavoriteFolder08)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_09, OnUpdateFavoriteFolder09)
    ON_UPDATE_COMMAND_UI(ID_FAVORITE_FOLDER_10, OnUpdateFavoriteFolder10)
    ON_COMMAND(ID_FAVORITE_FOLDER_01, OnFavoriteFolder01)
    ON_COMMAND(ID_FAVORITE_FOLDER_02, OnFavoriteFolder02)
    ON_COMMAND(ID_FAVORITE_FOLDER_03, OnFavoriteFolder03)
    ON_COMMAND(ID_FAVORITE_FOLDER_04, OnFavoriteFolder04)
    ON_COMMAND(ID_FAVORITE_FOLDER_05, OnFavoriteFolder05)
    ON_COMMAND(ID_FAVORITE_FOLDER_06, OnFavoriteFolder06)
    ON_COMMAND(ID_FAVORITE_FOLDER_07, OnFavoriteFolder07)
    ON_COMMAND(ID_FAVORITE_FOLDER_08, OnFavoriteFolder08)
    ON_COMMAND(ID_FAVORITE_FOLDER_09, OnFavoriteFolder09)
    ON_COMMAND(ID_FAVORITE_FOLDER_10, OnFavoriteFolder10)
    //}}AFX_MSG_MAP
    // 標準のファイル基本ドキュメント コマンド
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp クラスの構築

CSuperTagEditorApp::CSuperTagEditorApp()
{
    // TODO: この位置に構築用コードを追加してください。
    // ここに InitInstance 中の重要な初期化処理をすべて記述してください。
    m_hAccel = 0;
    m_accelTable = NULL;        // アクセラレータテーブル
    m_nAccelTable = 0;
}

CSuperTagEditorApp::~CSuperTagEditorApp()
{
    WriteRegistry();
    m_pRecentFileList->WriteList();
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSuperTagEditorApp オブジェクト

CSuperTagEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp クラスの初期化
// =============================================
// CSuperTagEditorApp::MakeFileName
// 概要  : 自アプリケーションのパスからファイル名を
//       : フルパスで作成する
// 引数  : sExt         = ファイル拡張子
// 戻り値: TCHAR *      = ファイル名(NULL=失敗)
// =============================================
TCHAR *CSuperTagEditorApp::MakeFileName(TCHAR *sExt)
{
    TCHAR   drive[_MAX_DRIVE];
    TCHAR   dir[_MAX_DIR];
    TCHAR   fname[_MAX_FNAME];
    TCHAR   buff[_MAX_PATH] = {0};
    TCHAR   buff_canonicalized[_MAX_PATH];//by Kobarin(C:\\.\ste\SuperTagEditor.exe のようになることがある)
    //自己アプリのパス所得（大小文字識別付き）
    GetModuleFileName(m_hInstance, buff, _MAX_PATH);
    PathCanonicalize(buff_canonicalized, buff);
    WIN32_FIND_DATA wfd;

    HANDLE  h = ::FindFirstFile(buff_canonicalized, &wfd);
    if (h == NULL) return(NULL);

    _tsplitpath_s(buff_canonicalized, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
    _tmakepath_s(buff_canonicalized, drive, dir, wfd.cFileName, NULL);
    ::FindClose(h);

    _tsplitpath_s(buff_canonicalized, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, 0);
    _tmakepath_s(buff_canonicalized, drive, dir, fname, sExt);

    return(_tcsdup(buff_canonicalized));
}


BOOL CSuperTagEditorApp::InitInstance()
{
    // 標準的な初期化処理
    // もしこれらの機能を使用せず、実行ファイルのサイズを小さく
    // したければ以下の特定の初期化ルーチンの中から不必要なもの
    // を削除してください。

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // 多重起動禁止処理 /* FreeFall 045 */

    // 設定が保存される下のレジストリ キーを変更します。
    // TODO: この文字列を、会社名または所属など適切なものに
    // 変更してください。
    //SetRegistryKey(_T("MERCURY"));
    free((void *)m_pszProfileName);
    m_pszProfileName = MakeFileName(_T("ini"));
    { /* STEP 031 */
        BOOL bFlag = FALSE;
        for (int i = 1; i < __argc; i++) {
            LPCTSTR pszParam = __targv[i];
            if (bFlag) {
                free((void *)m_pszProfileName);
                m_pszProfileName = _tcsdup(pszParam);
                break;
            }
            if (pszParam[0] == _T('-') || pszParam[0] == _T('/'))
            {
                if (_tcscmp(pszParam, _T("-I")) == 0 || _tcscmp(pszParam, _T("/I")) == 0) {
                    bFlag = TRUE;
                }
            }
        }
    }
    m_IniFile.Open(m_pszProfileName);
    ReadRegistry();

    LoadStdProfileSettings(g_nRecentFolder);  // 標準の INI ファイルのオプションをロードします (MRU を含む)  /* StartInaction 053 */

    // 多重起動禁止処理 /* FreeFall 045 */
    if (!g_bValidDupExec) {
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, STEP_MUTEX_NAME);
        if(hMutex){
    //      MessageBox(NULL, "既に "PROG_NAME" は起動中です", "多重起動エラー", MB_ICONSTOP|MB_OK|MB_TOPMOST);
            // 起動済みの STE にファイル名・フォルダ名を渡す
            if(__argc >= 2){
                KbDDEClient ddeClient(NULL, STEP_DDE_SERVICE_NAME, STEP_DDE_TOPIC_NAME);
                TCHAR szCommand[2048];
                int i;
                for(i = 1; i < __argc; i++){
                    _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\""), __targv[i]);//ファイル名を "" で括る
                    if(!ddeClient.Execute(szCommand, 3000)){
                    //失敗したらそれ以上は渡さない
                        break;
                    }
                }
            }
            CloseHandle(hMutex);
            return FALSE;
        }
        m_hMutex = CreateMutex(FALSE, 0, STEP_MUTEX_NAME);
    }

    // アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
    //  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CSuperTagEditorDoc),
        RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
        RUNTIME_CLASS(CSuperTagEditorView));
    AddDocTemplate(pDocTemplate);

    // DDE Execute open を使用可能にします。
    EnableShellOpen();
//  RegisterShellFileTypes(TRUE);

    // DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // 最大化起動対応
    if (g_bMainFrameZoomed) {
        CWinApp::m_nCmdShow = SW_SHOWMAXIMIZED;
    } else if (g_bMainFrameIconic) {
        CWinApp::m_nCmdShow = SW_SHOWMINIMIZED;
    }

    // コマンドラインでディスパッチ コマンドを指定します。
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;
    // ProcessShellCommand()で扱われなかったものをここで処理
    { /* Misirlou 140 */
        BOOL bFirst = TRUE;
        for (int i = 1; i < __argc; i++) {
            LPCTSTR pszParam = __targv[i];
            BOOL bFlag = FALSE;
            BOOL bLast = ((i + 1) == __argc);
            if (pszParam[0] == _T('-') || pszParam[0] == _T('/'))
            {
                // remove flag specifier
                bFlag = TRUE;
                ++pszParam;
            }
            if (!bFlag && !bFirst) {
                OpenDocumentFile(pszParam);

            }
            if (!bFlag) bFirst = FALSE;
        }
    }

    // メイン ウィンドウが初期化されたので、表示と更新を行います。
    if (g_bMainFrameZoomed) {
//      m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
//      m_pMainWnd->UpdateWindow();
    } else if (g_bMainFrameIconic) {
//      m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
//      m_pMainWnd->UpdateWindow();
    } else {
        m_pMainWnd->ShowWindow(SW_SHOW);
        m_pMainWnd->UpdateWindow();
    }

    // ドラッグ/ドロップ オープンを許可します
//  m_pMainWnd->DragAcceptFiles();
    //VS2022 ランタイムの有無を確認
#ifdef _WIN64
    HINSTANCE hDll = LoadLibrary(_T("vcruntime140_1.dll"));
#else
    HINSTANCE hDll = LoadLibrary(_T("vcruntime140.dll"));
#endif
#if 0
#ifdef _DEBUG
    if(hDll){
        FreeLibrary(hDll);hDll = NULL;
    }
#endif
#endif
    if(!hDll){//インストールされていない
        MessageBox(NULL,
                   _T("VS2022 ランタイムがインストールされていません。\n")
                   _T("STEP_J-h の動作には VS2022 ランタイムのインストールが必要です。\n")
                   _T("既にインストール済みなのにこのダイアログが表示される場合はランタイムの\r\n")
                   _T("バージョンが古い可能性があります。最新版をインストールし直して下さい。\r\n" )
                   _T("インストールしないと標準のプラグインを使用出来ないため、何も出来ません。\n")
                   _T("64bit OS では 64bit 版と 32bit 版の両方のランタイムをインストールすることをお勧めします。\n")
                   _T("OK をクリックすると配布サイトにジャンプします。\n"),
                   _T("エラー"), MB_OK|MB_TOPMOST);
        ShellExecute(NULL, _T("open"), VCRUNTIME_URL, 0, 0, SW_SHOWNORMAL);
    }
    else{//OK
        FreeLibrary(hDll);
    }

    return TRUE;
}

int CSuperTagEditorApp::ExitInstance()
{
    // TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
    ReleaseMutex(m_hMutex);

    // アクセラレータテーブルの解放
    DestroyAccelerator();

    delete [] g_genreListUSER;
    CoUninitialize();

    return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// ダイアログ データ
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    CString m_strVersion;
    //}}AFX_DATA

    // ClassWizard 仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
    //{{AFX_MSG(CAboutDlg)
        // メッセージ ハンドラはありません。
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT

    // バージョン情報の設定
    //m_strVersion.Format(PROG_NAME" Ver "PROG_VERSION"(Build:%d) 改", BUILDCOUNT_NUM);
    //m_strVersion.Format(PROG_NAME_ORG2 " (" PROG_NAME_ORG " Ver " PROG_VERSION_ORG "改) Ver " PROG_VERSION_ORG2 " 改\n\n" PROG_NAME " Version " PROG_VERSION);
    m_strVersion = _T("五代目+h SuperTagEditor\n") PROG_NAME _T(" Version ") PROG_VERSION _T(" (") PROG_PLATFORM _T(")\n")
                   _T("\n")
                   PROG_COPYRIGHT _T("\n")
                   PROG_URL       _T("\n") ;
                   //PROG_MAIL ; STEP_J メールアドレスを表示する場合は有効にして下さい。
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Text(pDX, IDC_ST_VERSION, m_strVersion);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // メッセージ ハンドラはありません。
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CSuperTagEditorApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSuperTagEditorApp メッセージ ハンドラ
static const TCHAR  sSectionOption[]        = _T("Option");
static const TCHAR  sKeyVersion[]           = _T("Version");
static const TCHAR  sKeyCurrentDir[]        = _T("CurrentDir");
static const TCHAR  sKeyCurrentPlayList[]   = _T("CurrentPlayList");
static const TCHAR  sKeyEnterBeginEdit[]    = _T("EnterBeginEdit");
static const TCHAR  sKeyESCEditCancel[]     = _T("ESCEditCancel");
static const TCHAR  sKeyEnableEditCursorExit[] = _T("EnableEditCursorExit");
static const TCHAR  sKeyEditOkDown[]        = _T("EditOkDown");
static const TCHAR  sKeyKeepTimeStamp[]     = _T("KeepTimeStamp");
static const TCHAR  sKeySyncCreateTime[]    = _T("SyncCreateTime");
static const TCHAR  sKeyChangeFileExt[]     = _T("ChangeFileExt");
static const TCHAR  sKeyLoadFileChecked[]   = _T("LoadFileChecked");
static const TCHAR  sKeyHideMP3ListFile[]   = _T("HideMP3ListFile");
static const TCHAR  sKeyDropSearchSubFolder[] = _T("DropSearchSubFolder");    /* TyphoonSwell 026 */
static const TCHAR  sKeyShowZenSpace[]      = _T("ShowZenSpace");   /* BeachMonster 107 */
static const TCHAR  sKeyShowOtherChar[]     = _T("ShowOtherChar");  /* BeachMonster 107 */
static const TCHAR  sKeySortIgnoreCase[]    = _T("SortIgnoreCase"); /* BeachMonster4 114 */
static const TCHAR  sKeySortIgnoreZenHan[]  = _T("SortIgnoreZenHan");   /* BeachMonster4 114 */
static const TCHAR  sKeySortIgnoreKataHira[]= _T("SortIgnoreKataHira"); /* FunnyCorn 179 */
static const TCHAR  sKeyShowTotalParent[]   = _T("ShowTotalParent");    /* RockDance 128 */
static const TCHAR  sKeyShowTips[]          = _T("ShowTips");   /* Rumble 188 */
static const TCHAR  sKeyChangeTextFile[]    = _T("ChangeTextFile");
static const TCHAR  sKeyEditFiledSIF[]      = _T("EditFiledSI");
static const TCHAR  sKeyAutoOpenFolder[]    = _T("AutoOpenFolder");
static const TCHAR  sKeyLoadFileAdjustColumn[]  = _T("LoadFileAdjustColumn");
static const TCHAR  sKeySetLyricsDir[]      = _T("SetLyricsDir");
static const TCHAR  sKeySearchLyricsSubDir[]= _T("SearchLyricsSubDir");
static const TCHAR  sKeyEnableSearchSubDir[]= _T("EnableSearchSubDir");
static const TCHAR  sKeyLyricsPath[]        = _T("LyricsPath");
static const TCHAR  sKeyCheckFileName[]     = _T("CheckFileName");
static const TCHAR  sKeyPlayerType[]        = _T("PlayerType");
static const TCHAR  sKeyWinAmpPath[]        = _T("WinAmpPath");

static const TCHAR  sKeyID3v2GenreAddNumber[] = _T("ID3v2GenreAddNumber");
static const TCHAR  sKeyFileNameMaxCheck[]  = _T("FileNameMaxCheck");
static const TCHAR  sKeyFileNameMaxChar[]   = _T("FileNameMaxChar");
static const TCHAR  sKeyFileNameMaxCellColor[] = _T("FileNameMaxCellColor");

static const TCHAR  sSectionFolderSync[]    = _T("FolderSync");
static const TCHAR  sKeyEnableFolderSync[]  = _T("EnableFolderSync");
static const TCHAR  sKeySyncRootFolder[]    = _T("RootFolder");
static const TCHAR  sKeySyncSelectAlways[]  = _T("SelectAlways");
static const TCHAR  sKeySyncDeleteFolder[]  = _T("DeleteFolder");
static const TCHAR  sKeySyncLyricsFileMove[]= _T("LyricsFileMove");

static const TCHAR  sSectionConfMessage[]   = _T("ConfMessage");
static const TCHAR  sKeyConfConvMP3[]       = _T("ConvMP3");
static const TCHAR  sKeyConfConvRMP[]       = _T("ConvRMP");
static const TCHAR  sKeyConfConvID3v2[]     = _T("ConvID3v2");
static const TCHAR  sKeyConfDeleteFile[]    = _T("DeleteFile");
static const TCHAR  sKeyConfDeleteList[]    = _T("DeleteList");
static const TCHAR  sKeyConfEditModify[]    = _T("EditModify");
static const TCHAR  sKeyConfFolderSync[]    = _T("FolderSync");

static const TCHAR  sSectionLoadPlayList[]  = _T("LoadPlayList");
static const TCHAR  sKeyClearList[]         = _T("ClearList");
static const TCHAR  sKeyClearCheck[]        = _T("ClearCheck");
static const TCHAR  sKeyAddList[]           = _T("AddList");
static const TCHAR  sKeyFileCheck[]         = _T("FileCheck");

static const TCHAR  *sSectionCheckWord[CHECK_STATE_MAX] = {_T("CheckWord"), _T("ReplaceWord")};
static const TCHAR  sKeySearchWord[]        = _T("SearchWord");
static const TCHAR  sKeyReplaceWord[]       = _T("ReplaceWord");
static const TCHAR  sKeyTargetColumn[]      = _T("TargetColumn");
static const TCHAR  sKeyCheckDiffUL[]       = _T("CheckDiffUL");
static const TCHAR  sKeyRegExp[]            = _T("RegExp");
static const TCHAR  sKeyRangeSelected[]     = _T("RangeSelected");
static const TCHAR  sKeyMatchComplete[]     = _T("MatchComplete");
static const TCHAR  sKeyMatchSelected[]     = _T("MatchSelected");

// ユーザー書式変換
static const TCHAR  sSectionUserConvFormat[] = _T("UserConvFormat");
static const TCHAR  sKeyConvFormatType[]     = _T("ConvFormatType");
static const TCHAR  sKeyUserFormName[]       = _T("Name");
static const TCHAR  sKeyUserFormTag2File[]   = _T("TagToFile");
static const TCHAR  sKeyUserFormFile2Tag[]   = _T("FileToTag");

// 拡張版書式変換
static const TCHAR  sSectionConvFormatEx[]   = _T("ConvFormatEx");
//static const TCHAR  sKeyUserFormName[]       = _T("Name");
static const TCHAR  sKeyUserFormat[]         = _T("Format");
static const TCHAR  sKeyUserFixString[]      = _T("FixString");
static const TCHAR  sKeyUserInitNumber[]     = _T("InitNumber");
static const TCHAR  sKeyUserAddNumber[]      = _T("AddNumber");
static const TCHAR  sKeyUserColumnCount[]    = _T("ColumnCount");
static const TCHAR  sKeyUserSpaceInit[]      = _T("SpaceInit");

// 移動先フォルダ書式
static const TCHAR  sSectionMoveFolder[]       = _T("MoveFolderFormat");
//static const TCHAR  sKeyUserFormName[]        = _T("Name");
static const TCHAR  sKeyMoveFolderFormat[]     = _T("Format");
static const TCHAR  sKeyMoveFolderFixString[]  = _T("FixString");
static const TCHAR  sKeyMoveFolderCopy[]       = _T("Copy");
static const TCHAR  sKeyMoveFolderInitFolder[] = _T("InitFolder");

// 書式コピー /* FunnyCorn 175 */
static const TCHAR  sSectionCopyFormat[]       = _T("CopyFormatFormat");
//static const TCHAR  sKeyUserFormName[]         = _T("Name");
static const TCHAR  sKeyCopyFormatFormat[]     = _T("Format");
static const TCHAR  sKeyCopyFormatFixString[]  = _T("FixString");

// タグ情報変換 /* STEP 034 */
static const TCHAR  sSectionConvFormatTag2Tag[]= _T("ConvFormatTag2Tag");
static const TCHAR  sKeyUserTag2TagFormName[]  = _T("Name");
static const TCHAR  sKeyUserTagTagFormat[]     = _T("Format");

// 定型文貼り付け /* SeaKnows 030 */
static const TCHAR  sSectionTeikei[]           = _T("Teikei");
static const TCHAR  sSectionTeikeiGroupName[]  = _T("TeikeiGroupName");
static const TCHAR  sSectionTeikeiPaste[]      = _T("TeikeiPaste");
static const TCHAR  sSectionTeikeiAddSpace[]   = _T("TeikeiAddSpace");
static const TCHAR  sSectionTeikeiAddChar[]    = _T("TeikeiAddChar");
static const TCHAR  sSectionTeikeiFront[]      = _T("TeikeiAddFront");
static const TCHAR  sSectionTeikeiBack[]       = _T("TeikeiAddBack");
static const TCHAR  sSectionTeikeiShowDialog[] = _T("TeikeiShowDialog");

// フォルダ単一選択 /* SeaKnows 033 */
static const TCHAR  sSectionValidFolderSelect[]= _T("ValidFolderSelect");
// 多重起動を許可する /* FreeFall 045 */
static const TCHAR  sSectionValidDupExec[]     = _T("ValidDupExec");
// 最近使ったフォルダの数
static const TCHAR  sSectionRecentFolderNum[]  = _T("RecentFolderNum");
// 検索・置換ダイアログの位置を記憶する
static const TCHAR  sSectionSaveRepDlgPos[]    = _T("SaveRepDlgPos");
static const TCHAR  sSectionSaveRepDlgPosX[]   = _T("SaveRepDlgPosX");
static const TCHAR  sSectionSaveRepDlgPosY[]   = _T("SaveRepDlgPosY");
// 下方向に連番を追加ダイアログの設定 /* Baja 159 */
static const TCHAR  sSectionAddNumberWidth[]   = _T("AddNumberWidth");
static const TCHAR  sSectionAddNumberPos[]     = _T("AddNumberPos");
static const TCHAR  sSectionAddNumberSep[]     = _T("AddNumberSep");
static const TCHAR  sSectionAddNumberBef[]     = _T("AddNumberBef"); /* Conspiracy 194 */
static const TCHAR  sSectionAddNumberAft[]     = _T("AddNumberAft"); /* Conspiracy 194 */
// Audio Listに表示されるフォルダ名などの分類をセルサイズを無視して(実際は隣３セル分まで)表示する
static const TCHAR  sSectionAudioListShow[]    = _T("AudioListShow"); /* Conspiracy 199 */

// フォント設定
static const TCHAR  sSectionFont[]           = _T("Font");
static const TCHAR  sKeyFontFace[]           = _T("Face");
static const TCHAR  sKeyFontHeight[]         = _T("Height");
static const TCHAR  sKeyFontWidth[]          = _T("Width");
static const TCHAR  sKeyFontEscapement[]     = _T("Escapement");
static const TCHAR  sKeyFontOrientation[]    = _T("Orientation");
static const TCHAR  sKeyFontWeight[]         = _T("Weight");
static const TCHAR  sKeyFontItalic[]         = _T("Italic");
static const TCHAR  sKeyFontUnderline[]      = _T("Underline");
static const TCHAR  sKeyFontStrikeOut[]      = _T("StrikeOut");
static const TCHAR  sKeyFontCharSet[]        = _T("CharSet");
static const TCHAR  sKeyFontOutPrecision[]   = _T("OutPrecision");
static const TCHAR  sKeyFontClipPrecision[]  = _T("ClipPrecision");
static const TCHAR  sKeyFontQuality[]        = _T("Quality");
static const TCHAR  sKeyFontPitchAndFamily[] = _T("PitchAndFamily");

// リスト出力書式
static const TCHAR  sSectionWriteFormat[]    = _T("WriteFormat");
static const TCHAR  sKeyWriteFormName[]      = _T("Name");
static const TCHAR  sKeyWriteFileName[]      = _T("FileName");
static const TCHAR  sKeyWriteExtName[]       = _T("ExtName");
static const TCHAR  sKeyWriteSelected[]      = _T("WriteSelected");
static const TCHAR  sKeyWriteCurrentFile[]   = _T("CurrentFile");
static const TCHAR  sKeyWriteIsHtml[]        = _T("IsHtml");
static const TCHAR  sKeyWriteHtml[]          = _T("WriteHtml"); /* BeachMonster5 120 */

static const TCHAR  sSectionRepFileName[]    = _T("RepFileName");
static const TCHAR  sKeyRepCharAfter[]       = _T("RepCharAfter");
static const TCHAR  sKeyRepCharBefore[]      = _T("RepCharBefore");  /* FreeFall 050 */

static const TCHAR  sSectionSort[]       = _T("Sort");
static const TCHAR  sKeySortColumn[]     = _T("Column");
static const TCHAR  sKeySortType[]       = _T("Type");

static const TCHAR  sSectionClass[]      = _T("Class");
static const TCHAR  sKeyClassType[]      = _T("Type");
static const TCHAR  sKeyClassColumn[]    = _T("Column");

static const TCHAR  sSectionWindow[]     = _T("Window");
static const TCHAR  sKeyWinZoomed[]      = _T("Zoomed");
static const TCHAR  sKeyWinIconic[]      = _T("Iconic");
static const TCHAR  sKeyMainWindow[]     = _T("MainWindow");
static const TCHAR  sKeySplitSize[]      = _T("SplitSize");

static const TCHAR  sSectionKeyConfig[]  = _T("KeyConfig");

static const TCHAR  sSectionGenreList[]  = _T("GenreList");
static const TCHAR  sSectionUserGenreList[]  = _T("UserGenreList");
static const TCHAR  sKeyUserGenreAddList[]   = _T("AddList");
static const TCHAR  sKeyUserGenreNo[]    = _T("No");
static const TCHAR  sKeyUserGenreName[]  = _T("Name");

// 文字種統一 /* StartInaction 054 */
static const TCHAR  sKeyUnifyAlpha[] = _T("UnifyAlpha");
static const TCHAR  sKeyUnifyHiraKata[]  = _T("UnifyHiraKata");
static const TCHAR  sKeyUnifyKata[]  = _T("UnifyKata");
static const TCHAR  sKeyUnifyKigou[] = _T("UnifyKigou");
static const TCHAR  sKeyUnifySuji[]  = _T("UnifySuji");
static const TCHAR  sKeyUnifyUpLow[] = _T("UnifyUpLow");
static const TCHAR  sKeyUnifyFixedUpLow[]    = _T("UnifyFixedUpLow"); /* STEP 040 */

// 拡張子統一 /* STE 007 */
static const TCHAR  sKeyFileExtChange[] = _T("FileExtChange");

// ファイル名文字種統一 /* LastTrain 058 */
static const TCHAR  sKeyFileUnifyAlpha[] = _T("FileUnifyAlpha");
static const TCHAR  sKeyFileUnifyHiraKata[]  = _T("FileUnifyHiraKata");
static const TCHAR  sKeyFileUnifyKata[]  = _T("FileUnifyKata");
static const TCHAR  sKeyFileUnifyKigou[] = _T("FileUnifyKigou");
static const TCHAR  sKeyFileUnifySuji[]  = _T("FileUnifySuji");
static const TCHAR  sKeyFileUnifyUpLow[] = _T("FileUnifyUpLow");

// プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
static const TCHAR  sKeyShowLoadPlaylistDlg[] = _T("ShowLoadPlaylistDlg");

// お気に入りのフォルダ /* RockDance 129 */
static const TCHAR  sSectionFavorites[]          = _T("Favorites");

static const TCHAR  sKeyFirstUpperIgnoreWord[] = _T("FistUpperIgnoreWord");
static const TCHAR  sKeyFirstUpperIgnoreWords[] = _T("FistUpperIgnoreWords");
static const TCHAR  sKeyFirstUpperSentenceSeparator[] = _T("FistUpperSentenceSeparator");
static const TCHAR  sKeyUserConvAddMenu[] = _T("UserConvAddMenu");
static const TCHAR  sKeyZenHanKigouKana[] = _T("ZenHanKigouKana");

static const TCHAR  sKeyAutoTilde2WaveDash[] = _T("AutoTilde2WaveDash");//全角チルダを波ダッシュに自動置換

void CSuperTagEditorApp::ReadWindowStatus(const TCHAR *sKey, RECT *rect)
{
    static const TCHAR sDefault[] = _T("0 0 0 0");
    TCHAR buffer[2048];
    m_IniFile.ReadStr(sSectionWindow, sKey, sDefault, buffer, _countof(buffer));
    _stscanf_s(buffer, _T("%d %d %d %d"), &rect->left,
                                          &rect->top,
                                          &rect->right,
                                          &rect->bottom);
}

void CSuperTagEditorApp::WriteWindowStatus(const TCHAR *sKey, RECT *rect)
{
    CString str;
    str.Format(_T("%d %d %d %d"), rect->left,
                              rect->top,
                              rect->right,
                              rect->bottom);
    m_IniFile.WriteStr(sSectionWindow, sKey, str);
}

void CSuperTagEditorApp::ReadRegistry(void)
{
    int     i;

    // 各種設定を読み込む
    CString strVersion;
    TCHAR buf[2048];
    strVersion = m_IniFile.ReadStr(sSectionOption, sKeyVersion, _T(""), buf, _countof(buf));
    g_bIsVersionUp = _tcscmp(strVersion, PROG_VERSION) ? true : false;

    g_strCurrentDirectory   = m_IniFile.ReadStr(sSectionOption, sKeyCurrentDir, _T(""), buf, _countof(buf));
    g_strCurrentPlayList    = m_IniFile.ReadStr(sSectionOption, sKeyCurrentPlayList, _T("*.m3u"), buf, _countof(buf));

    // 一般 - ユーザーインターフェース
    g_bOptESCEditCancel         = m_IniFile.ReadInt(sSectionOption, sKeyESCEditCancel, 0) ? true : false;
    g_bOptEnableEditCursorExit  = m_IniFile.ReadInt(sSectionOption, sKeyEnableEditCursorExit, 1) ? true : false;
    g_bOptEditOkDown        = m_IniFile.ReadInt(sSectionOption, sKeyEditOkDown, 1) ? true : false;
    g_bOptEnterBeginEdit    = m_IniFile.ReadInt(sSectionOption, sKeyEnterBeginEdit, 0) ? true : false;

    // 一般 - 動作設定
    g_bOptKeepTimeStamp     = m_IniFile.ReadInt(sSectionOption, sKeyKeepTimeStamp, 0) ? true : false;
    g_bOptSyncCreateTime    = m_IniFile.ReadInt(sSectionOption, sKeySyncCreateTime, 0) ? true : false;
    g_bOptChangeFileExt     = m_IniFile.ReadInt(sSectionOption, sKeyChangeFileExt, 0) ? true : false;
    g_bOptAutoOpenFolder    = m_IniFile.ReadInt(sSectionOption, sKeyAutoOpenFolder, 0) ? true : false;
    g_bOptLoadFileAdjustColumn  = m_IniFile.ReadInt(sSectionOption, sKeyLoadFileAdjustColumn, 1) ? true : false;
    g_bOptLoadFileChecked   = m_IniFile.ReadInt(sSectionOption, sKeyLoadFileChecked, 1) ? true : false;
    g_bOptHideMP3ListFile   = m_IniFile.ReadInt(sSectionOption, sKeyHideMP3ListFile, 0) ? true : false;

    // 一般 - 歌詞ファイル
    g_bOptChangeTextFile        = m_IniFile.ReadInt(sSectionOption, sKeyChangeTextFile, 1) ? true : false;
    g_bOptSetLyricsDir          = m_IniFile.ReadInt(sSectionOption, sKeySetLyricsDir, 0) ? true : false;
    g_bOptSearchLyricsSubDir    = m_IniFile.ReadInt(sSectionOption, sKeySearchLyricsSubDir, 0) ? true : false;
    g_strOptLyricsPath          = m_IniFile.ReadStr(sSectionOption, sKeyLyricsPath, _T(""), buf, _countof(buf));

    //
    g_bOptEditFieldSIF      = m_IniFile.ReadInt(sSectionOption, sKeyEditFiledSIF, 1) ? true : false;
    g_nOptCheckFileName     = m_IniFile.ReadInt(sSectionOption, sKeyCheckFileName, FILENAME_CONV_MULTIBYTE);
    g_bEnableSearchSubDir   = m_IniFile.ReadInt(sSectionOption, sKeyEnableSearchSubDir, 1) ? true : false;

    // フォルダの同期
    g_bEnableFolderSync     = m_IniFile.ReadInt(sSectionFolderSync, sKeyEnableFolderSync, 0) ? true : false;
    g_strRootFolder         = m_IniFile.ReadStr(sSectionFolderSync, sKeySyncRootFolder, _T(""), buf, _countof(buf));
    g_bSyncSelectAlways     = m_IniFile.ReadInt(sSectionFolderSync, sKeySyncSelectAlways, 0) ? true : false;
    g_bSyncDeleteFolder     = m_IniFile.ReadInt(sSectionFolderSync, sKeySyncDeleteFolder, 1) ? true : false;
    g_bSyncLyricsFileMove   = m_IniFile.ReadInt(sSectionFolderSync, sKeySyncLyricsFileMove, 1) ? true : false;

    // 確認メッセージ表示
    g_bConfDeleteFile   = m_IniFile.ReadInt(sSectionConfMessage, sKeyConfDeleteFile, 1) ? true : false;
    g_bConfDeleteList   = m_IniFile.ReadInt(sSectionConfMessage, sKeyConfDeleteList, 1) ? true : false;
    g_bConfEditModify   = m_IniFile.ReadInt(sSectionConfMessage, sKeyConfEditModify, 1) ? true : false;
    g_bConfFolderSync   = m_IniFile.ReadInt(sSectionConfMessage, sKeyConfFolderSync, 1) ? true : false;

    // プレイリスト
    g_bPlayListClearList    = m_IniFile.ReadInt(sSectionLoadPlayList, sKeyClearList, 0) ? true : false;
    g_bPlayListClearCheck   = m_IniFile.ReadInt(sSectionLoadPlayList, sKeyClearCheck, 1) ? true : false;
    g_bPlayListAddList      = true;//m_IniFile.ReadInt(sSectionLoadPlayList, sKeyAddList, 1) ? true : false;//初期値変更(なんのために無効にするのだろうか？)
    g_bPlayListFileCheck    = m_IniFile.ReadInt(sSectionLoadPlayList, sKeyFileCheck, 1) ? true : false;

    // 条件チェックの状態
    for (i = 0; i < CHECK_STATE_MAX; i++) {
        CHECK_WORD_STATE    *pState = &g_chkWord[i];
        const TCHAR *sSectionName = sSectionCheckWord[i];
        pState->strSearchWord   = m_IniFile.ReadStr(sSectionName, sKeySearchWord, _T(""), buf, _countof(buf));
        pState->strReplaceWord  = m_IniFile.ReadStr(sSectionName, sKeyReplaceWord, _T(""), buf, _countof(buf));
        pState->nTargetColumn   = m_IniFile.ReadInt(sSectionName, sKeyTargetColumn, -1);
        pState->bCheckDiffUL    = m_IniFile.ReadInt(sSectionName, sKeyCheckDiffUL, 0) ? true : false;
        pState->bRegExp         = m_IniFile.ReadInt(sSectionName, sKeyRegExp, 0) ? true : false;
        pState->bRangeSelected  = m_IniFile.ReadInt(sSectionName, sKeyRangeSelected, 0) ? true : false;
        pState->bMatchComplete  = m_IniFile.ReadInt(sSectionName, sKeyMatchComplete, 0) ? true : false;
        pState->bMatchSelected  = m_IniFile.ReadInt(sSectionName, sKeyMatchSelected, 0) ? true : false;
    }

    // ユーザー変換書式
    const TCHAR *sDefFormatTag2File = _T("%ARTIST_NAME%-%ALBUM_NAME%-%TRACK_NAME%");
    const TCHAR *sDefFormatFile2Tag = sDefFormatTag2File;
    //g_nUserConvFormatType = m_IniFile.ReadInt(sSectionUserConvFormat, sKeyConvFormatType, 0);
    { /* 数を増やしたので本家併用時に影響を与えないように LastTrain 057 */
        CString     strSectionName;
        strSectionName = _T("haseta\\");
        strSectionName += sSectionUserConvFormat;
        g_nUserConvFormatType   = m_IniFile.ReadInt(strSectionName, sKeyConvFormatType, 0);
    }
    for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
        CString     strKeyName;
        // 名称
        strKeyName.Format(_T("%s%d"), sKeyUserFormName, i);
        g_userConvFormat[i].strName = m_IniFile.ReadStr(sSectionUserConvFormat, strKeyName, _T("名称未設定"), buf, _countof(buf));
        // タグ情報 => ファイル名
        strKeyName.Format(_T("%s%d"), sKeyUserFormTag2File, i);
        g_userConvFormat[i].strTag2File = m_IniFile.ReadStr(sSectionUserConvFormat, strKeyName, sDefFormatTag2File, buf, _countof(buf));
        // ファイル名 => タグ情報
        strKeyName.Format(_T("%s%d"), sKeyUserFormFile2Tag, i);
        g_userConvFormat[i].strFile2Tag = m_IniFile.ReadStr(sSectionUserConvFormat, strKeyName, sDefFormatFile2Tag, buf, _countof(buf));
        // ２つ目以降はデフォルト文字列はクリア
        sDefFormatTag2File = sDefFormatFile2Tag = _T("");
    }

    // 拡張版ユーザー変換書式
    for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("%s%d"), sSectionConvFormatEx, i);
        // 名称
        g_userConvFormatEx[i].strName = m_IniFile.ReadStr(strSectionName, sKeyUserFormName, _T("名称未設定"), buf, _countof(buf));
        // 書式
        g_userConvFormatEx[i].strFormat = m_IniFile.ReadStr(strSectionName, sKeyUserFormat, _T(""), buf, _countof(buf));
        // 固定文字列
        g_userConvFormatEx[i].strFixString = m_IniFile.ReadStr(strSectionName, sKeyUserFixString, _T(""), buf, _countof(buf));
        // 連番：初期値
        g_userConvFormatEx[i].nInitNumber = m_IniFile.ReadInt(strSectionName, sKeyUserInitNumber, 1);
        // 連番：加算値
        g_userConvFormatEx[i].nAddNumber = m_IniFile.ReadInt(strSectionName, sKeyUserAddNumber, 1);
        // 連番：桁数
        g_userConvFormatEx[i].nColumnCount = m_IniFile.ReadInt(strSectionName, sKeyUserColumnCount, 1);
        // 書き込み不可のセルで連番クリア
        g_userConvFormatEx[i].bSpaceInitNumber = m_IniFile.ReadInt(strSectionName, sKeyUserSpaceInit, 0) ? true : false;
    }

    // 移動先フォルダ書式
    for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionMoveFolder, i);
        // 名称
        g_userMoveFolder[i].strName = m_IniFile.ReadStr(strSectionName, sKeyUserFormName, _T("名称未設定"), buf, _countof(buf));
        // 書式
        g_userMoveFolder[i].strFormat = m_IniFile.ReadStr(strSectionName, sKeyMoveFolderFormat, _T(""), buf, _countof(buf));
        // 固定文字列
        g_userMoveFolder[i].strFixString = m_IniFile.ReadStr(strSectionName, sKeyMoveFolderFixString, _T(""), buf, _countof(buf));
        // コピー
        g_userMoveFolder[i].bCopy = m_IniFile.ReadInt(strSectionName, sKeyMoveFolderCopy, 0) ? true : false;
        // 初期フォルダ
        g_userMoveFolder[i].strInitFolder = m_IniFile.ReadStr(strSectionName, sKeyMoveFolderInitFolder, _T(""), buf, _countof(buf)); /* STEP 022 */
    }

    // 書式コピー /* FunnyCorn 175 */
    for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionCopyFormat, i);
        // 名称
        g_userCopyFormat[i].strName = m_IniFile.ReadStr(strSectionName, sKeyUserFormName, _T("名称未設定"), buf, _countof(buf));
        // 書式
        g_userCopyFormat[i].strFormat = m_IniFile.ReadStr(strSectionName, sKeyCopyFormatFormat, _T(""), buf, _countof(buf));
        // 固定文字列
        g_userCopyFormat[i].strFixString = m_IniFile.ReadStr(strSectionName, sKeyCopyFormatFixString, _T(""), buf, _countof(buf));
    }

    // タグ情報変換 /* STEP 034 */
    for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionConvFormatTag2Tag, i);
        // 名称
        g_userConvFormatTag2Tag[i].strName = m_IniFile.ReadStr(strSectionName, sKeyUserTag2TagFormName, _T("名称未設定"), buf, _countof(buf));
        // 書式
        g_userConvFormatTag2Tag[i].strFormat = m_IniFile.ReadStr(strSectionName, sKeyUserTagTagFormat, _T(""), buf, _countof(buf));
    }

    {
        CString     sSectionOption;
        sSectionOption =_T("haseta");
        //g_bOptID3v2GenreAddNumber = m_IniFile.ReadInt(sSectionOption, sKeyID3v2GenreAddNumber, 1) ? true : false;
        g_bConfFileNameMaxCheck = m_IniFile.ReadInt(sSectionOption, sKeyFileNameMaxCheck, 0) ? true : false;
        g_bFileNameMaxCellColor = m_IniFile.ReadInt(sSectionOption, sKeyFileNameMaxCellColor, 0) ? true : false; /* SeaKnows 036 */
        g_nConfFileNameMaxChar  = m_IniFile.ReadInt(sSectionOption, sKeyFileNameMaxChar, 255);
        g_bOptDropSearchSubFolder   = m_IniFile.ReadInt(sSectionOption, sKeyDropSearchSubFolder, 0) ? true : false;   /* TyphoonSwell 026 */
        g_bOptShowZenSpace  = m_IniFile.ReadInt(sSectionOption, sKeyShowZenSpace, 1) ? true : false;  /* BeachMonster 107 */
        g_sOptShowOtherChar = m_IniFile.ReadStr(sSectionOption, sKeyShowOtherChar, _T(""), buf, _countof(buf));
        g_bOptSortIgnoreCase    = m_IniFile.ReadInt(sSectionOption, sKeySortIgnoreCase, 0) ? true : false;    /* BeachMonster4 114 */
        g_bOptSortIgnoreZenHan  = m_IniFile.ReadInt(sSectionOption, sKeySortIgnoreZenHan, 0) ? true : false;  /* BeachMonster4 114 */
        g_bOptSortIgnoreKataHira= m_IniFile.ReadInt(sSectionOption, sKeySortIgnoreKataHira, 0) ? true : false;    /* FunnyCorn 179 */
        g_bOptShowTotalParent   = m_IniFile.ReadInt(sSectionOption, sKeyShowTotalParent, 0) ? true : false;   /* RockDance 128 */
        g_bOptShowTips  = m_IniFile.ReadInt(sSectionOption, sKeyShowTips, 1) ? true : false;  /* Rumble 188 */
    }
    // 定型文貼り付け /* SeaKnows 030 *//* FreeFall 046 */
    for (int k=0;k<3;k++) {
        CString     sSectionOption;
        CString     strSectionName;
        sSectionOption = _T("haseta");
        strSectionName.Format(_T("%s%d"), sSectionTeikeiGroupName, k);
        g_strTeikeiGroupName[k] = m_IniFile.ReadStr(sSectionOption, strSectionName, _T("名称未設定"), buf, _countof(buf));
        for (i = 0; i < 10; i++) {
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikei, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikei, k, i);
            }
            g_teikeiInfo[i+k*10].strTeikei = m_IniFile.ReadStr(sSectionOption, strSectionName, _T(""), buf, _countof(buf)); /* STEP 035 */
            /* STEP 035 */
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiPaste, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiPaste, k, i);
            }
            g_teikeiInfo[i+k*10].nTeikeiPaste = m_IniFile.ReadInt(sSectionOption, strSectionName, 0);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiAddSpace, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiAddSpace, k, i);
            }
            g_teikeiInfo[i+k*10].bAddSpace = m_IniFile.ReadInt(sSectionOption, strSectionName, 0) ? true : false;
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiAddChar, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiAddChar, k, i);
            }
            g_teikeiInfo[i+k*10].bAddChar = m_IniFile.ReadInt(sSectionOption, strSectionName, 0) ? true : false;
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiFront, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiFront, k, i);
            }
            g_teikeiInfo[i+k*10].strFront = m_IniFile.ReadStr(sSectionOption, strSectionName, _T(""), buf, _countof(buf));
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiBack, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiBack, k, i);
            }
            g_teikeiInfo[i+k*10].strBack = m_IniFile.ReadStr(sSectionOption, strSectionName, _T(""), buf, _countof(buf));
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiShowDialog, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiShowDialog, k, i);
            }
            g_teikeiInfo[i+k*10].bShowDialog = m_IniFile.ReadInt(sSectionOption, strSectionName, 1) ? true : false;
        }
    }
    // ソフトウェア他 /* SeaKnows 031 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        g_bValidFolderSelect = m_IniFile.ReadInt(strSectionName, sSectionValidFolderSelect, 0) ? true : false;
        g_bValidDupExec = m_IniFile.ReadInt(strSectionName, sSectionValidDupExec, 0) ? true : false; /* FreeFall 045 */
        g_nRecentFolder = m_IniFile.ReadInt(strSectionName, sSectionRecentFolderNum, 5); /* 053 */
        g_bSaveRepDlgPos = m_IniFile.ReadInt(strSectionName, sSectionSaveRepDlgPos, 0) ? true : false; /* WildCherry4 086 */
        g_nSaveRepDlgPosX = m_IniFile.ReadInt(strSectionName, sSectionSaveRepDlgPosX, -1); /* WildCherry4 086 */
        g_nSaveRepDlgPosY = m_IniFile.ReadInt(strSectionName, sSectionSaveRepDlgPosY, -1); /* WildCherry4 086 */
        g_nAddNumberWidth = m_IniFile.ReadInt(strSectionName, sSectionAddNumberWidth, 2); /* Baja 159 */
        g_nAddNumberPos = m_IniFile.ReadInt(strSectionName, sSectionAddNumberPos, 0); /* Baja 159 */
        g_strAddNumberSep = m_IniFile.ReadStr(strSectionName, sSectionAddNumberSep, _T(""), buf, _countof(buf)); /* Baja 159 */
        g_strAddNumberBef = m_IniFile.ReadStr(strSectionName, sSectionAddNumberBef, _T(""), buf, _countof(buf)); /* Conspiracy 194 */
        g_strAddNumberAft = m_IniFile.ReadStr(strSectionName, sSectionAddNumberAft, _T(""), buf, _countof(buf)); /* Conspiracy 194 */
        g_bAudioListShow = m_IniFile.ReadInt(strSectionName, sSectionAudioListShow, 0) ? true : false; /* Conspiracy 199 */
    }

    // リスト出力書式
    for (i = 0; i < WRITE_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("%s%d"), sSectionWriteFormat, i);
        // 名称
        g_writeFormat[i].strName = m_IniFile.ReadStr(strSectionName, sKeyWriteFormName, _T("名称未設定"), buf, _countof(buf));
        // 書式ファイル名
        g_writeFormat[i].strFileName = m_IniFile.ReadStr(strSectionName, sKeyWriteFileName, _T(""), buf, _countof(buf));
        // 拡張子
        g_writeFormat[i].strExtName = m_IniFile.ReadStr(strSectionName, sKeyWriteExtName, _T(".txt"), buf, _countof(buf));
        // 選択ファイルのみ出力
        g_writeFormat[i].bWriteSelected = m_IniFile.ReadInt(strSectionName, sKeyWriteSelected, 0) ? true : false;
        // カレントファイル名
        g_writeFormat[i].strCurrentFile = m_IniFile.ReadStr(strSectionName, sKeyWriteCurrentFile, _T(""), buf, _countof(buf));
        // HTML ファイル出力用(空の場合に全角スペースを出力)
        g_writeFormat[i].bIsHtml = m_IniFile.ReadInt(strSectionName, sKeyWriteIsHtml, 0) ? true : false;
        { /* BeachMonster5 120 */
            g_writeFormat[i].bWriteHtml = m_IniFile.ReadInt(_T("haseta\\") + strSectionName, sKeyWriteHtml, 0) ? true : false;
        }
    }

    // ファイル名置換文字
    for (i = 0; i < FILENAME_REPLACE_MAX; i++) {
        FILENAME_REPLACE    *pRep = &g_fileNameReplace[i];
        CString     strKeyName;
        // 置換後文字列
        strKeyName.Format(_T("%s%d"), sKeyRepCharAfter, i);
        pRep->strBefore = g_sRepTable[i][0];
        pRep->strAfter = m_IniFile.ReadStr(sSectionRepFileName, strKeyName, g_sRepTable[i][1], buf, _countof(buf));
    }
    // ユーザファイル名置換文字 /* FreeFall 050 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta\\User");
        strSectionName += sSectionRepFileName;
        for (i = 0; i < USER_FILENAME_REPLACE_MAX; i++) {
            FILENAME_REPLACE    *pRep = &g_userFileNameReplace[i];
            CString     strKeyName;
            // 置換後文字列
            strKeyName.Format(_T("%s%d"), sKeyRepCharBefore, i);
            pRep->strBefore = m_IniFile.ReadStr(strSectionName, strKeyName, _T(""), buf, _countof(buf));
            strKeyName.Format(_T("%s%d"), sKeyRepCharAfter, i);
            pRep->strAfter = m_IniFile.ReadStr(strSectionName, strKeyName, _T(""), buf, _countof(buf));
        }
    }

    // 分類設定情報
    g_classInfo.nType = m_IniFile.ReadInt(sSectionClass, sKeyClassType, 0);
    for (i = 0; i < CLASS_MAX; i++) {
        CString     strKeyName;
        // カラム番号
        strKeyName.Format(_T("%s%d"), sKeyClassColumn, i);
        g_classInfo.nColumn[i] = m_IniFile.ReadInt(sSectionClass, strKeyName, -1);
    }

    // ソート情報
    for (i = 0; i < SORT_KEY_MAX; i++) {
        CString     strKeyName;
        SORT_STATE  *state = &g_sortState[i];
        // キー(カラム)
        strKeyName.Format(_T("%s%d"), sKeySortColumn, i);
        state->nKeyColumn = m_IniFile.ReadInt(sSectionSort, strKeyName, -1);
        // タイプ
        strKeyName.Format(_T("%s%d"), sKeySortType, i);
        state->nType = m_IniFile.ReadInt(sSectionSort, strKeyName, 0);
    }

    // ウィンドウのフォントを読み込む
    CString strFont;
    strFont = m_IniFile.ReadStr(sSectionFont, sKeyFontFace, _T("Segoe UI"), buf, _countof(buf));
    _tcsncpy_s(g_fontReport.lfFaceName, (const TCHAR *)strFont, _TRUNCATE);
    g_fontReport.lfHeight           = m_IniFile.ReadInt(sSectionFont, sKeyFontHeight, -MulDiv(9, CClientDC(0).GetDeviceCaps(LOGPIXELSY), 72));
    g_fontReport.lfWidth            = m_IniFile.ReadInt(sSectionFont, sKeyFontWidth, 0);
    g_fontReport.lfEscapement       = m_IniFile.ReadInt(sSectionFont, sKeyFontEscapement, 0);
    g_fontReport.lfOrientation      = m_IniFile.ReadInt(sSectionFont, sKeyFontOrientation, 0);
    g_fontReport.lfWeight           = m_IniFile.ReadInt(sSectionFont, sKeyFontWeight, 0);
    g_fontReport.lfItalic           = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontItalic, 0);
    g_fontReport.lfUnderline        = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontUnderline, 0);
    g_fontReport.lfStrikeOut        = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontStrikeOut, 0);
    g_fontReport.lfCharSet          = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontCharSet, 0);
    g_fontReport.lfOutPrecision     = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontOutPrecision, 0);
    g_fontReport.lfClipPrecision    = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontClipPrecision, 0);
    g_fontReport.lfQuality          = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontQuality, 0);
    g_fontReport.lfPitchAndFamily   = (unsigned char)m_IniFile.ReadInt(sSectionFont, sKeyFontPitchAndFamily, 0);

    //g_nOptPlayerType      = m_IniFile.ReadInt(sSectionOption, sKeyPlayerType, PLAYER_WINAMP);
    //g_sOptWinAmpPath      = m_IniFile.ReadStr(sSectionOption, sKeyWinAmpPath, "");
    { /* 数を増やしたので本家併用時に影響を与えないように WildCherry 070 */
        CString     strSectionName;
        strSectionName = _T("haseta\\");
        strSectionName += sSectionOption;
        g_nOptPlayerType    = m_IniFile.ReadInt(strSectionName, sKeyPlayerType, PLAYER_WINAMP);
        g_sOptWinAmpPath        = m_IniFile.ReadStr(strSectionName, sKeyWinAmpPath, _T(""), buf, _countof(buf));
    }
    if (g_sOptWinAmpPath.IsEmpty()) {
        // WinAmp のパスを取得
        GetWinampPath();
    }

    g_bMainFrameZoomed      = m_IniFile.ReadInt(sSectionWindow, sKeyWinZoomed, FALSE);
    g_bMainFrameIconic      = m_IniFile.ReadInt(sSectionWindow, sKeyWinIconic, FALSE);
    ReadWindowStatus(sKeyMainWindow, &g_rectMainWindow);

    // ユーザ指定ジャンル
    if (g_genreListUSER == NULL) {
        g_genreListUSER = new USER_GENRE_LIST[USER_GENRE_LIST_MAX];
    }

    // 文字種の統一 /* StartInaction 054 */
    {
        CString     strSectionName;
        strSectionName = "haseta";
        g_nUnifyAlpha = m_IniFile.ReadInt(strSectionName, sKeyUnifyAlpha, 0);
        g_nUnifyHiraKata = m_IniFile.ReadInt(strSectionName, sKeyUnifyHiraKata, 0);
        g_nUnifyKata = m_IniFile.ReadInt(strSectionName, sKeyUnifyKata, 0);
        g_nUnifyKigou = m_IniFile.ReadInt(strSectionName, sKeyUnifyKigou, 0);
        g_nUnifySuji = m_IniFile.ReadInt(strSectionName, sKeyUnifySuji, 0);
        g_nUnifyUpLow = m_IniFile.ReadInt(strSectionName, sKeyUnifyUpLow, 0);
        g_nUnifyFixedUpLow = m_IniFile.ReadInt(strSectionName, sKeyUnifyFixedUpLow, 0); /* STEP 040 */
    }

    // ファイル名文字種の統一 /* LastTrain 058 */
    {
        CString     strSectionName;
        strSectionName = "haseta";
        g_nFileUnifyAlpha = m_IniFile.ReadInt(strSectionName, sKeyFileUnifyAlpha, 0);
        g_nFileUnifyHiraKata = m_IniFile.ReadInt(strSectionName, sKeyFileUnifyHiraKata, 0);
        g_nFileUnifyKata = m_IniFile.ReadInt(strSectionName, sKeyFileUnifyKata, 0);
        g_nFileUnifyKigou = m_IniFile.ReadInt(strSectionName, sKeyFileUnifyKigou, 0);
        g_nFileUnifySuji = m_IniFile.ReadInt(strSectionName, sKeyFileUnifySuji, 0);
        g_nFileUnifyUpLow = m_IniFile.ReadInt(strSectionName, sKeyFileUnifyUpLow, 0);
    }

    // 拡張子の統一 /* STEP 006 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        g_nFileExtChange = m_IniFile.ReadInt(strSectionName, sKeyFileExtChange, 0);
    }

    // プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        g_bShowLoadPlaylistDlg = m_IniFile.ReadInt(strSectionName, sKeyShowLoadPlaylistDlg, 1) ? true : false;
    }

    // お気に入りのフォルダ /* RockDance 129 */
    {
        CString     sSectionOption;
        CString     strSectionName;
        sSectionOption = _T("haseta");
        for (i = 0; i < 10; i++) {
            strSectionName.Format(_T("%s%d"), sSectionFavorites, i);
            g_strFavorite[i] = m_IniFile.ReadStr(sSectionOption, strSectionName, _T(""), buf, _countof(buf));
        }
    }

    { /* STEP 026 */
        CString     strSectionName;
        strSectionName = "haseta";
        g_bFirstUpperIgnoreWord = m_IniFile.ReadInt(strSectionName, sKeyFirstUpperIgnoreWord, 0) ? true : false;
        g_strFirstUpperIgnoreWords = m_IniFile.ReadStr(sSectionOption,
                                                       sKeyFirstUpperIgnoreWords,
                                                       _T("a,an,and,at,by,for,in,into,of,on,or,the,to,with"),
                                                       buf, _countof(buf));
        g_strFirstUpperSentenceSeparator = m_IniFile.ReadStr(sSectionOption,
                                                       sKeyFirstUpperSentenceSeparator, _T("."), buf, _countof(buf));
        g_bUserConvAddMenu = m_IniFile.ReadInt(strSectionName, sKeyUserConvAddMenu, 0) ? true : false;
        g_bZenHanKigouKana = m_IniFile.ReadInt(strSectionName, sKeyZenHanKigouKana, 0) ? true : false;

        g_bAutoTilde2WaveDash = m_IniFile.ReadInt(strSectionName, sKeyAutoTilde2WaveDash, 0) ? true : false;
    }

    //Profile_Free();
}

void CSuperTagEditorApp::WriteRegistry(void)
{
    int     i;
    // 各種設定を書き込む
    m_IniFile.WriteStr(sSectionOption, sKeyVersion        , CString(PROG_VERSION) + CString("M"));
    m_IniFile.WriteStr(sSectionOption, sKeyCurrentDir     , g_strCurrentDirectory);
    m_IniFile.WriteStr(sSectionOption, sKeyCurrentPlayList, g_strCurrentPlayList);
    //dlgLoadProgress.SetPos(5);

    // 一般 - ユーザーインターフェース
    m_IniFile.WriteInt(sSectionOption, sKeyESCEditCancel       , g_bOptESCEditCancel ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyEnableEditCursorExit, g_bOptEnableEditCursorExit ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyEditOkDown          , g_bOptEditOkDown ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyEnterBeginEdit      , g_bOptEnterBeginEdit ? 1 : 0);
    //dlgLoadProgress.SetPos(10);

    // 一般 - 動作設定
    m_IniFile.WriteInt(sSectionOption, sKeyKeepTimeStamp       , g_bOptKeepTimeStamp ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeySyncCreateTime      , g_bOptSyncCreateTime ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyChangeFileExt       , g_bOptChangeFileExt ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyAutoOpenFolder      , g_bOptAutoOpenFolder ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyLoadFileAdjustColumn, g_bOptLoadFileAdjustColumn ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyLoadFileChecked     , g_bOptLoadFileChecked ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyHideMP3ListFile     , g_bOptHideMP3ListFile ? 1 : 0);
    //dlgLoadProgress.SetPos(15);

    // 一般 - 歌詞ファイル
    m_IniFile.WriteInt(sSectionOption, sKeyChangeTextFile    , g_bOptChangeTextFile ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeySetLyricsDir      , g_bOptSetLyricsDir ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeySearchLyricsSubDir, g_bOptSearchLyricsSubDir ? 1 : 0);
    m_IniFile.WriteStr(sSectionOption, sKeyLyricsPath     , g_strOptLyricsPath);
    //dlgLoadProgress.SetPos(20);

    //
    m_IniFile.WriteInt(sSectionOption, sKeyEditFiledSIF, g_bOptEditFieldSIF ? 1 : 0);
    m_IniFile.WriteInt(sSectionOption, sKeyCheckFileName, g_nOptCheckFileName);
    m_IniFile.WriteInt(sSectionOption, sKeyEnableSearchSubDir, g_bEnableSearchSubDir ? 1 : 0);
    //dlgLoadProgress.SetPos(25);

    // フォルダの同期
    m_IniFile.WriteInt(sSectionFolderSync, sKeyEnableFolderSync, g_bEnableFolderSync ? 1 : 0);
    m_IniFile.WriteStr(sSectionFolderSync, sKeySyncRootFolder, g_strRootFolder);
    m_IniFile.WriteInt(sSectionFolderSync, sKeySyncSelectAlways, g_bSyncSelectAlways ? 1 : 0);
    m_IniFile.WriteInt(sSectionFolderSync, sKeySyncDeleteFolder, g_bSyncDeleteFolder ? 1 : 0);
    m_IniFile.WriteInt(sSectionFolderSync, sKeySyncLyricsFileMove, g_bSyncLyricsFileMove ? 1 : 0);
    //dlgLoadProgress.SetPos(30);

    // 確認メッセージ表示
    m_IniFile.WriteInt(sSectionConfMessage, sKeyConfDeleteFile, g_bConfDeleteFile ? 1 : 0);
    m_IniFile.WriteInt(sSectionConfMessage, sKeyConfDeleteList, g_bConfDeleteList ? 1 : 0);
    m_IniFile.WriteInt(sSectionConfMessage, sKeyConfEditModify, g_bConfEditModify ? 1 : 0);
    m_IniFile.WriteInt(sSectionConfMessage, sKeyConfFolderSync, g_bConfFolderSync ? 1 : 0);
    //dlgLoadProgress.SetPos(35);

    // プレイリスト
    m_IniFile.WriteInt(sSectionLoadPlayList, sKeyClearList , g_bPlayListClearList ? 1 : 0);
    m_IniFile.WriteInt(sSectionLoadPlayList, sKeyClearCheck, g_bPlayListClearCheck ? 1 : 0);
    m_IniFile.WriteInt(sSectionLoadPlayList, sKeyAddList   , g_bPlayListAddList ? 1 : 0);
    m_IniFile.WriteInt(sSectionLoadPlayList, sKeyFileCheck , g_bPlayListFileCheck ? 1 : 0);
    //dlgLoadProgress.SetPos(40);

    // 条件チェックの状態
    for (i = 0; i < CHECK_STATE_MAX; i++) {
        CHECK_WORD_STATE    *pState = &g_chkWord[i];
        const TCHAR *sSectionName = sSectionCheckWord[i];
        m_IniFile.WriteStr(sSectionName, sKeySearchWord, pState->strSearchWord);
        m_IniFile.WriteStr(sSectionName, sKeyReplaceWord, pState->strReplaceWord);
        m_IniFile.WriteInt(sSectionName, sKeyTargetColumn , pState->nTargetColumn);
        m_IniFile.WriteInt(sSectionName, sKeyCheckDiffUL  , pState->bCheckDiffUL ? 1 : 0);
        m_IniFile.WriteInt(sSectionName, sKeyRegExp       , pState->bRegExp ? 1 : 0);
        m_IniFile.WriteInt(sSectionName, sKeyRangeSelected, pState->bRangeSelected ? 1 : 0);
        m_IniFile.WriteInt(sSectionName, sKeyMatchComplete, pState->bMatchComplete ? 1 : 0);
        m_IniFile.WriteInt(sSectionName, sKeyMatchSelected, pState->bMatchSelected ? 1 : 0);
    }
    //dlgLoadProgress.SetPos(45);

    // ファイル名置換文字
    for (i = 0; i < FILENAME_REPLACE_MAX; i++) {
        FILENAME_REPLACE    *pRep = &g_fileNameReplace[i];
        CString     strKeyName;
        // 置換後文字列
        strKeyName.Format(_T("%s%d"), sKeyRepCharAfter, i);
        m_IniFile.WriteStr(sSectionRepFileName, strKeyName, pRep->strAfter);
    }
    //dlgLoadProgress.SetPos(50);
    // ユーザファイル名置換文字 /* FreeFall 050 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta\\User");
        strSectionName += sSectionRepFileName;
        for (i = 0; i < USER_FILENAME_REPLACE_MAX; i++) {
            FILENAME_REPLACE    *pRep = &g_userFileNameReplace[i];
            CString     strKeyName;
            // 置換後文字列
            strKeyName.Format(_T("%s%d"), sKeyRepCharBefore, i);
            m_IniFile.WriteStr(strSectionName, strKeyName, pRep->strBefore);
            strKeyName.Format(_T("%s%d"), sKeyRepCharAfter, i);
            m_IniFile.WriteStr(strSectionName, strKeyName, pRep->strAfter);
        }
    }
    //dlgLoadProgress.SetPos(55);

    // ユーザー変換書式
    //m_IniFile.WriteInt(sSectionUserConvFormat, sKeyConvFormatType, g_nUserConvFormatType);
    { /* 数を増やしたので本家併用時に影響を与えないように LastTrain 057 */
        CString     strSectionName;
        strSectionName = _T("haseta\\");
        strSectionName += sSectionUserConvFormat;
        m_IniFile.WriteInt(strSectionName, sKeyConvFormatType, g_nUserConvFormatType);
    }
    //dlgLoadProgress.SetPos(60);
    for (i = 0; i < USER_CONV_FORMAT_MAX; i++) {
        CString     strKeyName;
        // 名称
        strKeyName.Format(_T("%s%d"), sKeyUserFormName, i);
        m_IniFile.WriteStr(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strName);
        // タグ情報 => ファイル名
        strKeyName.Format(_T("%s%d"), sKeyUserFormTag2File, i);
        m_IniFile.WriteStr(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strTag2File);
        // ファイル名 => タグ情報
        strKeyName.Format(_T("%s%d"), sKeyUserFormFile2Tag, i);
        m_IniFile.WriteStr(sSectionUserConvFormat, strKeyName, g_userConvFormat[i].strFile2Tag);
    }
    //dlgLoadProgress.SetPos(65);

    // 拡張版ユーザー変換書式
    for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("%s%d"), sSectionConvFormatEx, i);
        // 名称
        m_IniFile.WriteStr(strSectionName, sKeyUserFormName, g_userConvFormatEx[i].strName);
        // 書式
        m_IniFile.WriteStr(strSectionName, sKeyUserFormat, g_userConvFormatEx[i].strFormat);
        // 固定文字列
        m_IniFile.WriteStr(strSectionName, sKeyUserFixString, g_userConvFormatEx[i].strFixString);
        // 連番：初期値
        m_IniFile.WriteInt(strSectionName, sKeyUserInitNumber, g_userConvFormatEx[i].nInitNumber);
        // 連番：加算値
        m_IniFile.WriteInt(strSectionName, sKeyUserAddNumber, g_userConvFormatEx[i].nAddNumber);
        // 連番：桁数
        m_IniFile.WriteInt(strSectionName, sKeyUserColumnCount, g_userConvFormatEx[i].nColumnCount);
        // 書き込み不可のセルで連番クリア
        m_IniFile.WriteInt(strSectionName, sKeyUserSpaceInit, g_userConvFormatEx[i].bSpaceInitNumber ? 1 : 0);
    }
    //dlgLoadProgress.SetPos(70);

    // 移動先フォルダ書式
    for (i = 0; i < USER_MOVE_FODLER_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionMoveFolder, i);
        // 名称
        m_IniFile.WriteStr(strSectionName, sKeyUserFormName, g_userMoveFolder[i].strName);
        // 書式
        m_IniFile.WriteStr(strSectionName, sKeyMoveFolderFormat, g_userMoveFolder[i].strFormat);
        // 固定文字列
        m_IniFile.WriteStr(strSectionName, sKeyMoveFolderFixString, g_userMoveFolder[i].strFixString);
        // コピー
        m_IniFile.WriteInt(strSectionName, sKeyMoveFolderCopy, g_userMoveFolder[i].bCopy ? 1 : 0);
        // 書式
        m_IniFile.WriteStr(strSectionName, sKeyMoveFolderInitFolder, g_userMoveFolder[i].strInitFolder); /* STEP 022 */
    }

    // タグ情報変換 /* STEP 034 */
    for (i = 0; i < USER_CONV_FORMAT_TAG2TAG_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionConvFormatTag2Tag, i);
        // 名称
        m_IniFile.WriteStr(strSectionName, sKeyUserTag2TagFormName, g_userConvFormatTag2Tag[i].strName);
        // 書式
        m_IniFile.WriteStr(strSectionName, sKeyUserTagTagFormat, g_userConvFormatTag2Tag[i].strFormat);
    }

    //dlgLoadProgress.SetPos(75);
    // 書式コピー /* FunnyCorn 175 */
    for (i = 0; i < USER_COPY_FORMAT_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s%d"), sSectionCopyFormat, i);
        // 名称
        m_IniFile.WriteStr(strSectionName, sKeyUserFormName, g_userCopyFormat[i].strName);
        // 書式
        m_IniFile.WriteStr(strSectionName, sKeyCopyFormatFormat, g_userCopyFormat[i].strFormat);
        // 固定文字列
        m_IniFile.WriteStr(strSectionName, sKeyCopyFormatFixString, g_userCopyFormat[i].strFixString);
    }
    //dlgLoadProgress.SetPos(80);
    {
        CString     sSectionOption;
        sSectionOption = _T("haseta");
//      m_IniFile.WriteInt(sSectionOption, sKeyID3v2GenreAddNumber, g_bOptID3v2GenreAddNumber ? 1 : 0);
        m_IniFile.WriteInt(sSectionOption, sKeyFileNameMaxCheck, g_bConfFileNameMaxCheck ? 1 : 0);
        m_IniFile.WriteInt(sSectionOption, sKeyFileNameMaxCellColor, g_bFileNameMaxCellColor ? 1 : 0);
        m_IniFile.WriteInt(sSectionOption, sKeyFileNameMaxChar, g_nConfFileNameMaxChar);
        m_IniFile.WriteInt(sSectionOption, sKeyDropSearchSubFolder, g_bOptDropSearchSubFolder ? 1 : 0); /* TyphoonSwell 026 */
        m_IniFile.WriteInt(sSectionOption, sKeyShowZenSpace, g_bOptShowZenSpace ? 1 : 0); /* BeachMonster 107 */
        //m_IniFile.WriteInt(sSectionOption, sKeyShowZenSpace, g_bOptShowZenSpace ? 1 : 0); /* BeachMonster 107 */
        m_IniFile.WriteInt(sSectionOption, sKeySortIgnoreCase, g_bOptSortIgnoreCase ? 1 : 0); /* BeachMonster4 114 */
        m_IniFile.WriteInt(sSectionOption, sKeySortIgnoreZenHan, g_bOptSortIgnoreZenHan ? 1 : 0); /* BeachMonster4 114 */
        m_IniFile.WriteInt(sSectionOption, sKeySortIgnoreKataHira, g_bOptSortIgnoreKataHira ? 1 : 0); /* FunnyCorn 179 */
        m_IniFile.WriteInt(sSectionOption, sKeyShowTotalParent, g_bOptShowTotalParent ? 1 : 0); /* RockDance 128 */
        m_IniFile.WriteInt(sSectionOption, sKeyShowTips, g_bOptShowTips ? 1 : 0); /* Rumble 188 */
    }
    //dlgLoadProgress.SetPos(85);
    // 定型文貼り付け /* SeaKnows 030 */
    for (int k=0;k<3;k++) {
        CString     sSectionOption;
        CString     strSectionName;
        sSectionOption = _T("haseta");
        strSectionName.Format(_T("%s%d"), sSectionTeikeiGroupName, k);
        m_IniFile.WriteStr(sSectionOption, strSectionName, g_strTeikeiGroupName[k]);
        for (i = 0; i < 10; i++) {
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikei, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikei, k, i);
            }
            m_IniFile.WriteStr(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strTeikei); /* STEP 035 */
            /* STEP 035 */
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiPaste, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiPaste, k, i);
            }
            m_IniFile.WriteInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].nTeikeiPaste);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiAddSpace, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiAddSpace, k, i);
            }
            m_IniFile.WriteInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bAddSpace ? 1 : 0);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiAddChar, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiAddChar, k, i);
            }
            m_IniFile.WriteInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bAddChar ? 1 : 0);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiFront, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiFront, k, i);
            }
            m_IniFile.WriteStr(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strFront);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiBack, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiBack, k, i);
            }
            m_IniFile.WriteStr(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].strBack);
            if (k == 0) {
                strSectionName.Format(_T("%s%d"), sSectionTeikeiShowDialog, i);
            } else {
                strSectionName.Format(_T("%s%d-%d"), sSectionTeikeiShowDialog, k, i);
            }
            m_IniFile.WriteInt(sSectionOption, strSectionName, g_teikeiInfo[i+k*10].bShowDialog ? 1 : 0);
        }
    }

    // その他 /* SeaKnows 031,033 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        m_IniFile.WriteInt(strSectionName, sSectionValidFolderSelect, g_bValidFolderSelect ? 1 : 0);
        m_IniFile.WriteInt(strSectionName, sSectionValidDupExec, g_bValidDupExec ? 1 : 0); /* FreeFall 045 */
        m_IniFile.WriteInt(strSectionName, sSectionRecentFolderNum, g_nRecentFolder); /* m_IniFile.WriteInt 053 */
        m_IniFile.WriteInt(strSectionName, sSectionSaveRepDlgPos, g_bSaveRepDlgPos ? 1 : 0); /* WildCherry4 086 */
        m_IniFile.WriteInt(strSectionName, sSectionSaveRepDlgPosX, g_nSaveRepDlgPosX); /* WildCherry4 086 */
        m_IniFile.WriteInt(strSectionName, sSectionSaveRepDlgPosY, g_nSaveRepDlgPosY); /* WildCherry4 086 */
        m_IniFile.WriteInt(strSectionName, sSectionAddNumberWidth, g_nAddNumberWidth); /* Baja 159 */
        m_IniFile.WriteInt(strSectionName, sSectionAddNumberPos, g_nAddNumberPos); /* Baja 159 */
        m_IniFile.WriteStr(strSectionName, sSectionAddNumberSep, g_strAddNumberSep); /* Baja 159 */
        m_IniFile.WriteStr(strSectionName, sSectionAddNumberBef, g_strAddNumberBef); /* Conspiracy 194 */
        m_IniFile.WriteStr(strSectionName, sSectionAddNumberAft, g_strAddNumberAft); /* Conspiracy 194 */
        m_IniFile.WriteInt(strSectionName, sSectionAudioListShow, g_bAudioListShow ? 1 : 0); /* Conspiracy 199 */
    }

    // リスト出力書式
    for (i = 0; i < WRITE_FORMAT_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("%s%d"), sSectionWriteFormat, i);
        // 名称
        m_IniFile.WriteStr(strSectionName, sKeyWriteFormName, g_writeFormat[i].strName);
        // 書式ファイル名
        m_IniFile.WriteStr(strSectionName, sKeyWriteFileName, g_writeFormat[i].strFileName);
        // 拡張子
        m_IniFile.WriteStr(strSectionName, sKeyWriteExtName, g_writeFormat[i].strExtName);
        // 選択ファイルのみ出力
        m_IniFile.WriteInt(strSectionName, sKeyWriteSelected, g_writeFormat[i].bWriteSelected ? 1 : 0);
        // カレントファイル名
        m_IniFile.WriteStr(strSectionName, sKeyWriteCurrentFile, g_writeFormat[i].strCurrentFile);
        // HTML ファイル出力用(空の場合に全角スペースを出力)
        m_IniFile.WriteInt(strSectionName, sKeyWriteIsHtml, g_writeFormat[i].bIsHtml ? 1 : 0);
        { /* BeachMonster5 120 */
            m_IniFile.WriteInt(_T("haseta\\") + strSectionName, sKeyWriteHtml, g_writeFormat[i].bWriteHtml ? 1 : 0);
        }
    }
    //dlgLoadProgress.SetPos(90);

    // 分類設定情報
    m_IniFile.WriteInt(sSectionClass, sKeyClassType, g_classInfo.nType);
    for (i = 0; i < CLASS_MAX; i++) {
        CString     strKeyName;
        // カラム番号
        strKeyName.Format(_T("%s%d"), sKeyClassColumn, i);
        m_IniFile.WriteInt(sSectionClass, strKeyName, g_classInfo.nColumn[i]);
    }

    // ソート情報
    for (i = 0; i < SORT_KEY_MAX; i++) {
        CString     strKeyName;
        SORT_STATE  *state = &g_sortState[i];
        // キー(カラム)
        strKeyName.Format(_T("%s%d"), sKeySortColumn, i);
        m_IniFile.WriteInt(sSectionSort, strKeyName, state->nKeyColumn);
        // タイプ
        strKeyName.Format(_T("%s%d"), sKeySortType, i);
        m_IniFile.WriteInt(sSectionSort, strKeyName, state->nType);
    }

    // ウィンドウのフォントを書き込む
    m_IniFile.WriteStr(sSectionFont, sKeyFontFace, g_fontReport.lfFaceName);
    m_IniFile.WriteInt(sSectionFont, sKeyFontHeight      , g_fontReport.lfHeight);
    m_IniFile.WriteInt(sSectionFont, sKeyFontWidth       , g_fontReport.lfWidth);
    m_IniFile.WriteInt(sSectionFont, sKeyFontEscapement  , g_fontReport.lfEscapement);
    m_IniFile.WriteInt(sSectionFont, sKeyFontOrientation , g_fontReport.lfOrientation);
    m_IniFile.WriteInt(sSectionFont, sKeyFontWeight      , g_fontReport.lfWeight);
    m_IniFile.WriteInt(sSectionFont, sKeyFontItalic      , g_fontReport.lfItalic);
    m_IniFile.WriteInt(sSectionFont, sKeyFontUnderline   , g_fontReport.lfUnderline);
    m_IniFile.WriteInt(sSectionFont, sKeyFontStrikeOut   , g_fontReport.lfStrikeOut);
    m_IniFile.WriteInt(sSectionFont, sKeyFontCharSet     , g_fontReport.lfCharSet);
    m_IniFile.WriteInt(sSectionFont, sKeyFontOutPrecision    , g_fontReport.lfOutPrecision);
    m_IniFile.WriteInt(sSectionFont, sKeyFontClipPrecision , g_fontReport.lfClipPrecision);
    m_IniFile.WriteInt(sSectionFont, sKeyFontQuality     , g_fontReport.lfQuality);
    m_IniFile.WriteInt(sSectionFont, sKeyFontPitchAndFamily, g_fontReport.lfPitchAndFamily);

    //m_IniFile.WriteInt(sSectionOption, sKeyPlayerType, (int)g_nOptPlayerType);
    //m_IniFile.WriteStr(sSectionOption, sKeyWinAmpPath, g_sOptWinAmpPath);
    { /* 数を増やしたので本家併用時に影響を与えないように WildCherry 070 */
        CString     strSectionName;
        strSectionName = _T("haseta\\");
        strSectionName += sSectionOption;
        m_IniFile.WriteInt(strSectionName, sKeyPlayerType, (int)g_nOptPlayerType);
        m_IniFile.WriteStr(strSectionName, sKeyWinAmpPath, g_sOptWinAmpPath);
    }
    //dlgLoadProgress.SetPos(95);

    // ウィンドウの状態を保存
    m_IniFile.WriteInt(sSectionWindow, sKeyWinZoomed, (int)g_bMainFrameZoomed);
    m_IniFile.WriteInt(sSectionWindow, sKeyWinIconic, (int)g_bMainFrameIconic);
    WriteWindowStatus(sKeyMainWindow, &g_rectMainWindow);

    // 文字種の統一 /* StartInaction 054 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        m_IniFile.WriteInt(strSectionName, sKeyUnifyAlpha, g_nUnifyAlpha);
        m_IniFile.WriteInt(strSectionName, sKeyUnifyHiraKata, g_nUnifyHiraKata);
        m_IniFile.WriteInt(strSectionName, sKeyUnifyKata, g_nUnifyKata);
        m_IniFile.WriteInt(strSectionName, sKeyUnifyKigou, g_nUnifyKigou);
        m_IniFile.WriteInt(strSectionName, sKeyUnifySuji, g_nUnifySuji);
        m_IniFile.WriteInt(strSectionName, sKeyUnifyUpLow, g_nUnifyUpLow);
        m_IniFile.WriteInt(strSectionName, sKeyUnifyFixedUpLow, g_nUnifyFixedUpLow); /* STEP 040 */
    }

    // ファイル名文字種の統一 /* LastTrain 058 */
    {
        CString     strSectionName;
        strSectionName = "haseta";
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifyAlpha, g_nFileUnifyAlpha);
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifyHiraKata, g_nFileUnifyHiraKata);
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifyKata, g_nFileUnifyKata);
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifyKigou, g_nFileUnifyKigou);
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifySuji, g_nFileUnifySuji);
        m_IniFile.WriteInt(strSectionName, sKeyFileUnifyUpLow, g_nFileUnifyUpLow);
    }

    // 拡張子の統一 /* STEP 006 */
    {
        CString     strSectionName;
        strSectionName = "haseta";
        m_IniFile.WriteInt(strSectionName, sKeyFileExtChange, g_nFileExtChange);
    }
    // プレイリスト入力設定ダイアログを表示する /* RockDance 126 */
    {
        CString     strSectionName;
        strSectionName = _T("haseta");
        m_IniFile.WriteInt(strSectionName, sKeyShowLoadPlaylistDlg, g_bShowLoadPlaylistDlg ? 1 : 0);
    }
    //dlgLoadProgress.SetPos(100);
    // お気に入りのフォルダ /* RockDance 129 */
    {
        CString     sSectionOption;
        CString     strSectionName;
        sSectionOption = _T("haseta");
        for (i = 0; i < 10; i++) {
            strSectionName.Format(_T("%s%d"), sSectionFavorites, i);
            m_IniFile.WriteStr(sSectionOption, strSectionName, g_strFavorite[i]);
        }
    }

    { /* STEP 026 */
        CString     strSectionName;
        strSectionName = _T("haseta");
        m_IniFile.WriteInt(strSectionName, sKeyFirstUpperIgnoreWord, g_bFirstUpperIgnoreWord ? 1 : 0);
        m_IniFile.WriteStr(sSectionOption, sKeyFirstUpperIgnoreWords, g_strFirstUpperIgnoreWords);
        m_IniFile.WriteStr(sSectionOption, sKeyFirstUpperSentenceSeparator, g_strFirstUpperSentenceSeparator);
        m_IniFile.WriteInt(strSectionName, sKeyUserConvAddMenu, g_bUserConvAddMenu ? 1 : 0);
        m_IniFile.WriteInt(strSectionName, sKeyZenHanKigouKana, g_bZenHanKigouKana ? 1 : 0);
        m_IniFile.WriteInt(strSectionName, sKeyAutoTilde2WaveDash, g_bAutoTilde2WaveDash ? 1 : 0);
    }

    /*
    if (dlgLoadProgress.GetSafeHwnd() != NULL) {
        dlgLoadProgress.DestroyWindow();
        dlgLoadProgress.SetCanceled(FALSE);
    }
    */
    m_IniFile.Flush();
//  m_pRecentFileList->WriteList();
}

// キー割り当ての読み込み
void CSuperTagEditorApp::ReadKeyConfig(bool bUpdate)
{
    int     i;
    int     nTableMax = 0;
    for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
        KEY_CONFIG  *pKey = &g_listKeyConfig[i];
        if (pKey->wCmdID != 0x0000) {
            pKey->dwKeyCode = (DWORD)m_IniFile.ReadInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
            //pKey->dwKeyCode = (DWORD)Profile_GetInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
            if (pKey->dwKeyCode != 0) nTableMax++;
        }
    }

    extern CPlugin plugins;
    for (i=0;i<plugins.arPluginKey.GetSize();i++) {
        KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
        if (pKey->wCmdID != 0x0000) {
            pKey->dwKeyCode = (DWORD)m_IniFile.ReadInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
            //pKey->dwKeyCode = (DWORD)Profile_GetInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode, strINI);
            if (pKey->dwKeyCode != 0) nTableMax++;
        }
    }
    //FreeProfile();

    // アクセラレータキーテーブルの更新
    if (bUpdate) UpdateAccelerator(nTableMax);
}

// キー割り当ての保存
void CSuperTagEditorApp::WriteKeyConfig(bool bUpdate)
{
    int     i;
    int     nTableMax = 0;
    //CString sValue;
    for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
        KEY_CONFIG  *pKey = &g_listKeyConfig[i];
        if (pKey->wCmdID != 0x0000) {
            //sValue.Format("%d", pKey->dwKeyCode);
            //WritePrivateProfileString(sSectionKeyConfig, pKey->sRegName, sValue, strINI);
            m_IniFile.WriteInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
            if (pKey->dwKeyCode != 0) nTableMax++;
        }
    }

    extern CPlugin plugins;
    for (i=0;i<plugins.arPluginKey.GetSize();i++) {
        KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
        if (pKey->wCmdID != 0x0000) {
            //sValue.Format("%ld", pKey->dwKeyCode);
            //WritePrivateProfileString(sSectionKeyConfig, pKey->sRegName, sValue, strINI);
            m_IniFile.WriteInt(sSectionKeyConfig, pKey->sRegName, pKey->dwKeyCode);
            if (pKey->dwKeyCode != 0) nTableMax++;
        }
    }
    m_IniFile.Flush();
    // アクセラレータキーテーブルの更新
    if (bUpdate) UpdateAccelerator(nTableMax);
}

// アクセラレータテーブルの解放
void CSuperTagEditorApp::DestroyAccelerator(void)
{
    if (m_hAccel != 0) {
        DestroyAcceleratorTable(m_hAccel);
        m_hAccel = 0;
    }
    if (m_accelTable != NULL) {
        delete[]    m_accelTable;       // アクセラレータテーブル
        m_accelTable = NULL;
        m_nAccelTable = 0;
    }
}

// アクセラレータテーブルの更新
CMenu convSubMenuT2F;
CMenu convSubMenuF2T;
CString convSubMenuT2FTitle;
CString convSubMenuF2TTitle;
void CSuperTagEditorApp::UpdateAccelerator(int nTableMax)
{
    if (nTableMax == -1) {
        nTableMax = m_nAccelTable;
    }

    // アクセラレータテーブルの解放
    DestroyAccelerator();

    CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd(); /* STEP 030 *//* STEP 030 */
    CMenu   *pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;

    /* STEP 030 */
    pMenu->DestroyMenu();
    CMenu newMenu;
    newMenu.LoadMenu(IDR_MAINFRAME);
    pMainWnd->SetMenu(NULL);
    pMainWnd->SetMenu(&newMenu);
    pMainWnd->m_hMenuDefault = newMenu.m_hMenu;
    newMenu.Detach();

    /* STEP 030 */
    convSubMenuT2F.DestroyMenu();
    convSubMenuF2T.DestroyMenu();
    pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
    if (pMenu != NULL && g_bUserConvAddMenu) {
        CMenu* pUConvMenu = NULL;
        int nCount = pMenu->GetMenuItemCount();
        int j; for (int j=0;j<nCount;j++) {
            CString strTitle;
            pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
            if (strTitle == _T("変換(&C)")) {
                pMenu = pMenu->GetSubMenu(j);
                break;
            }
        }
        nCount = pMenu->GetMenuItemCount();
        for (j=0;j<nCount;j++) {
            CString strTitle;
            pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
            if (strTitle == _T("ユーザー指定変換(&U)")) {
                pUConvMenu = pMenu->GetSubMenu(j);
                break;
            }
        }
        convSubMenuT2F.LoadMenu(IDR_MENU_CONV_FORMAT_USER_T2F);
        pUConvMenu->GetMenuString(ID_CONV_TAG2FILE_USER, convSubMenuT2FTitle, MF_BYCOMMAND);
        pUConvMenu->ModifyMenu(ID_CONV_TAG2FILE_USER, MF_BYCOMMAND | MF_POPUP, (UINT_PTR)convSubMenuT2F.GetSubMenu(0)->GetSafeHmenu(), convSubMenuT2FTitle);
        //pUConvMenu->ModifyMenu(ID_CONV_TAG2FILE_USER, MF_BYCOMMAND | MF_STRING, ID_CONV_TAG2FILE_USER, convSubMenuT2FTitle);

        convSubMenuF2T.LoadMenu(IDR_MENU_CONV_FORMAT_USER_F2T);
        pUConvMenu->GetMenuString(ID_CONV_FILE2TAG_USER, convSubMenuF2TTitle, MF_BYCOMMAND);
        pUConvMenu->ModifyMenu(ID_CONV_FILE2TAG_USER, MF_BYCOMMAND | MF_POPUP, (UINT_PTR)convSubMenuF2T.GetSubMenu(0)->GetSafeHmenu(), convSubMenuF2TTitle);
        //pUConvMenu->ModifyMenu(ID_CONV_FILE2TAG_USER, MF_BYCOMMAND | MF_STRING, ID_CONV_FILE2TAG_USER, convSubMenuF2TTitle);
    }
    extern void OnLoadMainMenu();
    OnLoadMainMenu();

    pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
    // アクセラレータテーブル作成
    m_accelTable = new ACCEL[nTableMax];
    m_nAccelTable = nTableMax;
    int     i, nCount = 0;
    for (i = 0; g_listKeyConfig[i].sName != NULL; i++) {
        KEY_CONFIG  *pKey = &g_listKeyConfig[i];
        if (pKey->wCmdID != 0x0000) {
            // アクセラレータの作成
            ACCEL   *pAccel = NULL;
            WORD    wModifiers = 0x0000;
            if (pKey->dwKeyCode != 0) {
                wModifiers = HIWORD(pKey->dwKeyCode);
                pAccel = &m_accelTable[nCount];
                pAccel->fVirt = FNOINVERT | FVIRTKEY;
                if (wModifiers & HOTKEYF_ALT    ) pAccel->fVirt |= FALT;
                if (wModifiers & HOTKEYF_CONTROL) pAccel->fVirt |= FCONTROL;
                if (wModifiers & HOTKEYF_SHIFT  ) pAccel->fVirt |= FSHIFT;
                pAccel->key = LOWORD(pKey->dwKeyCode);
                pAccel->cmd = pKey->wCmdID;
                nCount++;
            }

            // メニュー項目の書き換え
            if (pMenu != NULL) {
                CString strName;
                // メニュー項目名を取得
                switch(pKey->wCmdID) {
                case ID_CONV_FORMAT_EX_01:
                    strName.Format(_T("01：%s"), (LPCWSTR)g_userConvFormatEx[0].strName);
                    break;
                case ID_CONV_FORMAT_EX_02:
                    strName.Format(_T("02：%s"), (LPCWSTR)g_userConvFormatEx[1].strName);
                    break;
                case ID_CONV_FORMAT_EX_03:
                    strName.Format(_T("03：%s"), (LPCWSTR)g_userConvFormatEx[2].strName);
                    break;
                case ID_CONV_FORMAT_EX_04:
                    strName.Format(_T("04：%s"), (LPCWSTR)g_userConvFormatEx[3].strName);
                    break;
                case ID_CONV_FORMAT_EX_05:
                    strName.Format(_T("05：%s"), (LPCWSTR)g_userConvFormatEx[4].strName);
                    break;
                case ID_CONV_FORMAT_EX_06:
                    strName.Format(_T("06：%s"), (LPCWSTR)g_userConvFormatEx[5].strName);
                    break;
                case ID_CONV_FORMAT_EX_07:
                    strName.Format(_T("07：%s"), (LPCWSTR)g_userConvFormatEx[6].strName);
                    break;
                case ID_CONV_FORMAT_EX_08:
                    strName.Format(_T("08：%s"), (LPCWSTR)g_userConvFormatEx[7].strName);
                    break;
                case ID_CONV_FORMAT_EX_09:
                    strName.Format(_T("09：%s"), (LPCWSTR)g_userConvFormatEx[8].strName);
                    break;
                case ID_CONV_FORMAT_EX_10:
                    strName.Format(_T("10：%s"), (LPCWSTR)g_userConvFormatEx[9].strName);
                    break;
                case ID_WRITE_LIST1:
                    strName.Format(_T("01：%s"), (LPCWSTR)g_writeFormat[0].strName);
                    break;
                case ID_WRITE_LIST2:
                    strName.Format(_T("02：%s"), (LPCWSTR)g_writeFormat[1].strName);
                    break;
                case ID_WRITE_LIST3:
                    strName.Format(_T("03：%s"), (LPCWSTR)g_writeFormat[2].strName);
                    break;
                case ID_WRITE_LIST4:
                    strName.Format(_T("04：%s"), (LPCWSTR)g_writeFormat[3].strName);
                    break;
                case ID_WRITE_LIST5:
                    strName.Format(_T("05：%s"), (LPCWSTR)g_writeFormat[4].strName);
                    break;
                case ID_MOVE_FOLDER_01:
                    strName.Format(_T("01：%s"), (LPCWSTR)g_userMoveFolder[0].strName);
                    break;
                case ID_MOVE_FOLDER_02:
                    strName.Format(_T("02：%s"), (LPCWSTR)g_userMoveFolder[1].strName);
                    break;
                case ID_MOVE_FOLDER_03:
                    strName.Format(_T("03：%s"), (LPCWSTR)g_userMoveFolder[2].strName);
                    break;
                case ID_MOVE_FOLDER_04:
                    strName.Format(_T("04：%s"), (LPCWSTR)g_userMoveFolder[3].strName);
                    break;
                case ID_MOVE_FOLDER_05:
                    strName.Format(_T("05：%s"), (LPCWSTR)g_userMoveFolder[4].strName);
                    break;
                case ID_CONV_FORMAT_USER_01: /* TyphoonSwell 027 */
                case ID_CONV_FORMAT_USER_T2F_01: /* STEP 030 */
                case ID_CONV_FORMAT_USER_F2T_01: /* STEP 030 */
                    strName.Format(_T("01：%s"), (LPCWSTR)g_userConvFormat[0].strName);
                    break;
                case ID_CONV_FORMAT_USER_02: /* TyphoonSwell 027 */
                case ID_CONV_FORMAT_USER_T2F_02: /* STEP 030 */
                case ID_CONV_FORMAT_USER_F2T_02: /* STEP 030 */
                    strName.Format(_T("02：%s"), (LPCWSTR)g_userConvFormat[1].strName);
                    break;
                case ID_CONV_FORMAT_USER_03: /* TyphoonSwell 027 */
                case ID_CONV_FORMAT_USER_T2F_03: /* STEP 030 */
                case ID_CONV_FORMAT_USER_F2T_03: /* STEP 030 */
                    strName.Format(_T("03：%s"), (LPCWSTR)g_userConvFormat[2].strName);
                    break;
                case ID_CONV_FORMAT_USER_04: /* LastTrain 057 */
                case ID_CONV_FORMAT_USER_T2F_04: /* STEP 030 */
                case ID_CONV_FORMAT_USER_F2T_04: /* STEP 030 */
                    strName.Format(_T("04：%s"), (LPCWSTR)g_userConvFormat[3].strName);
                    break;
                case ID_CONV_FORMAT_USER_05: /* LastTrain 057 */
                case ID_CONV_FORMAT_USER_T2F_05: /* STEP 030 */
                case ID_CONV_FORMAT_USER_F2T_05: /* STEP 030 */
                    strName.Format(_T("05：%s"), (LPCWSTR)g_userConvFormat[4].strName);
                    break;
                case ID_TEIKEI_01: /* SeaKnows 030 */
                    strName.Format(_T("01: %s"), (LPCWSTR)(shortString(g_teikeiInfo[0].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_02: /* SeaKnows 030 */
                    strName.Format(_T("02: %s"), (LPCWSTR)(shortString(g_teikeiInfo[1].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_03: /* SeaKnows 030 */
                    strName.Format(_T("03: %s"), (LPCWSTR)(shortString(g_teikeiInfo[2].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_04: /* SeaKnows 030 */
                    strName.Format(_T("04: %s"), (LPCWSTR)(shortString(g_teikeiInfo[3].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_05: /* SeaKnows 030 */
                    strName.Format(_T("05: %s"), (LPCWSTR)(shortString(g_teikeiInfo[4].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_06: /* SeaKnows 030 */
                    strName.Format(_T("06: %s"), (LPCWSTR)(shortString(g_teikeiInfo[5].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_07: /* SeaKnows 030 */
                    strName.Format(_T("07: %s"), (LPCWSTR)(shortString(g_teikeiInfo[6].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_08: /* SeaKnows 030 */
                    strName.Format(_T("08: %s"), (LPCWSTR)(shortString(g_teikeiInfo[7].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_09: /* SeaKnows 030 */
                    strName.Format(_T("09: %s"), (LPCWSTR)(shortString(g_teikeiInfo[8].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_10: /* SeaKnows 030 */
                    strName.Format(_T("10: %s"), (LPCWSTR)(shortString(g_teikeiInfo[9].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_01: /* FreeFall 046 */
                    strName.Format(_T("01: %s"), (LPCWSTR)(shortString(g_teikeiInfo[10].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_02: /* FreeFall 046 */
                    strName.Format(_T("02: %s"), (LPCWSTR)(shortString(g_teikeiInfo[11].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_03: /* FreeFall 046 */
                    strName.Format(_T("03: %s"), (LPCWSTR)(shortString(g_teikeiInfo[12].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_04: /* FreeFall 046 */
                    strName.Format(_T("04: %s"), (LPCWSTR)(shortString(g_teikeiInfo[13].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_05: /* FreeFall 046 */
                    strName.Format(_T("05: %s"), (LPCWSTR)(shortString(g_teikeiInfo[14].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_06: /* FreeFall 046 */
                    strName.Format(_T("06: %s"), (LPCWSTR)(shortString(g_teikeiInfo[15].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_07: /* FreeFall 046 */
                    strName.Format(_T("07: %s"), (LPCWSTR)(shortString(g_teikeiInfo[16].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_08: /* FreeFall 046 */
                    strName.Format(_T("08: %s"), (LPCWSTR)(shortString(g_teikeiInfo[17].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_09: /* FreeFall 046 */
                    strName.Format(_T("09: %s"), (LPCWSTR)(shortString(g_teikeiInfo[18].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_2_10: /* FreeFall 046 */
                    strName.Format(_T("10: %s"), (LPCWSTR)(shortString(g_teikeiInfo[19].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_01: /* FreeFall 046 */
                    strName.Format(_T("01: %s"), (LPCWSTR)(shortString(g_teikeiInfo[20].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_02: /* FreeFall 046 */
                    strName.Format(_T("02: %s"), (LPCWSTR)(shortString(g_teikeiInfo[21].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_03: /* FreeFall 046 */
                    strName.Format(_T("03: %s"), (LPCWSTR)(shortString(g_teikeiInfo[22].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_04: /* FreeFall 046 */
                    strName.Format(_T("04: %s"), (LPCWSTR)(shortString(g_teikeiInfo[23].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_05: /* FreeFall 046 */
                    strName.Format(_T("05: %s"), (LPCWSTR)(shortString(g_teikeiInfo[24].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_06: /* FreeFall 046 */
                    strName.Format(_T("06: %s"), (LPCWSTR)(shortString(g_teikeiInfo[25].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_07: /* FreeFall 046 */
                    strName.Format(_T("07: %s"), (LPCWSTR)(shortString(g_teikeiInfo[26].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_08: /* FreeFall 046 */
                    strName.Format(_T("08: %s"), (LPCWSTR)(shortString(g_teikeiInfo[27].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_09: /* FreeFall 046 */
                    strName.Format(_T("09: %s"), (LPCWSTR)(shortString(g_teikeiInfo[28].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_TEIKEI_3_10: /* FreeFall 046 */
                    strName.Format(_T("10: %s"), (LPCWSTR)(shortString(g_teikeiInfo[29].strTeikei, TEIKEI_MENU_STRING_MAX)));/* STEP 035, FreeFall 051 */
                    break;
                case ID_FAVORITE_FOLDER_01: /* RockDance 129 */
                    strName.Format(_T("&1 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[0])));
                    break;
                case ID_FAVORITE_FOLDER_02: /* RockDance 129 */
                    strName.Format(_T("&2 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[1])));
                    break;
                case ID_FAVORITE_FOLDER_03: /* RockDance 129 */
                    strName.Format(_T("&3 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[2])));
                    break;
                case ID_FAVORITE_FOLDER_04: /* RockDance 129 */
                    strName.Format(_T("&4 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[3])));
                    break;
                case ID_FAVORITE_FOLDER_05: /* RockDance 129 */
                    strName.Format(_T("&5 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[4])));
                    break;
                case ID_FAVORITE_FOLDER_06: /* RockDance 129 */
                    strName.Format(_T("&6 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[5])));
                    break;
                case ID_FAVORITE_FOLDER_07: /* RockDance 129 */
                    strName.Format(_T("&7 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[6])));
                    break;
                case ID_FAVORITE_FOLDER_08: /* RockDance 129 */
                    strName.Format(_T("&8 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[7])));
                    break;
                case ID_FAVORITE_FOLDER_09: /* RockDance 129 */
                    strName.Format(_T("&9 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[8])));
                    break;
                case ID_FAVORITE_FOLDER_10: /* RockDance 129 */
                    strName.Format(_T("&0 %s"), (LPCWSTR)(shortFavorite(g_strFavorite[9])));
                    break;
                case ID_EDIT_COPY_FORMAT_01: /* FunnyCorn 175 */
                    strName.Format(_T("01：%s"), (LPCWSTR)g_userCopyFormat[0].strName);
                    break;
                case ID_EDIT_COPY_FORMAT_02: /* FunnyCorn 175 */
                    strName.Format(_T("02：%s"), (LPCWSTR)g_userCopyFormat[1].strName);
                    break;
                case ID_EDIT_COPY_FORMAT_03: /* FunnyCorn 175 */
                    strName.Format(_T("03：%s"), (LPCWSTR)g_userCopyFormat[2].strName);
                    break;
                case ID_EDIT_COPY_FORMAT_04: /* FunnyCorn 175 */
                    strName.Format(_T("04：%s"), (LPCWSTR)g_userCopyFormat[3].strName);
                    break;
                case ID_EDIT_COPY_FORMAT_05: /* FunnyCorn 175 */
                    strName.Format(_T("05：%s"), (LPCWSTR)g_userCopyFormat[4].strName);
                    break;
                case ID_CONV_TAG_TO_TAG_01: /* STEP 034 */
                    strName.Format(_T("01：%s"), (LPCWSTR)g_userConvFormatTag2Tag[0].strName);
                    break;
                case ID_CONV_TAG_TO_TAG_02: /* STEP 034 */
                    strName.Format(_T("02：%s"), (LPCWSTR)g_userConvFormatTag2Tag[1].strName);
                    break;
                case ID_CONV_TAG_TO_TAG_03: /* STEP 034 */
                    strName.Format(_T("03：%s"), (LPCWSTR)g_userConvFormatTag2Tag[2].strName);
                    break;
                case ID_CONV_TAG_TO_TAG_04: /* STEP 034 */
                    strName.Format(_T("04：%s"), (LPCWSTR)g_userConvFormatTag2Tag[3].strName);
                    break;
                case ID_CONV_TAG_TO_TAG_05: /* STEP 034 */
                    strName.Format(_T("05：%s"), (LPCWSTR)g_userConvFormatTag2Tag[4].strName);
                    break;
                default:
                    pMenu->GetMenuString((UINT)pKey->wCmdID, strName, MF_BYCOMMAND);
                    break;
                }

                // 現在のアクセラレータキー部分を削除
                int     nPos;
                if ((nPos = strName.Find(_T('\t'))) != -1) {
                    strName = strName.Left(nPos);
                }

                if (pAccel != NULL) {
                    // 新しいアクセラレータキー部分を追加
                    strName += _T('\t');
                    if (wModifiers & HOTKEYF_ALT    ) strName += _T("Alt+");
                    if (wModifiers & HOTKEYF_CONTROL) strName += _T("Ctrl+");
                    if (wModifiers & HOTKEYF_SHIFT  ) strName += _T("Shift+");
                    // キー部分を追加
                    if (pAccel->key <= 0x91) {
                        strName += g_sKeyName[pAccel->key];
                    } else {
                        strName += _T("?");
                    }
                }

                // メニュー文字列を更新
                pMenu->ModifyMenu((UINT)pKey->wCmdID, MF_BYCOMMAND, pKey->wCmdID, (LPCTSTR)strName);
            }
        }
    }

    extern CPlugin plugins;
    for (i=0;i<plugins.arPluginKey.GetSize();i++) {
        KEY_CONFIG* pKey = (KEY_CONFIG*)plugins.arPluginKey.GetAt(i);
        if (pKey->wCmdID != 0x0000) {
            // アクセラレータの作成
            ACCEL   *pAccel = NULL;
            WORD    wModifiers = 0x0000;
            if (pKey->dwKeyCode != 0) {
                wModifiers = HIWORD(pKey->dwKeyCode);
                pAccel = &m_accelTable[nCount];
                pAccel->fVirt = FNOINVERT | FVIRTKEY;
                if (wModifiers & HOTKEYF_ALT    ) pAccel->fVirt |= FALT;
                if (wModifiers & HOTKEYF_CONTROL) pAccel->fVirt |= FCONTROL;
                if (wModifiers & HOTKEYF_SHIFT  ) pAccel->fVirt |= FSHIFT;
                pAccel->key = LOWORD(pKey->dwKeyCode);
                pAccel->cmd = pKey->wCmdID;
                nCount++;
            }

            // メニュー項目の書き換え
            if (pMenu != NULL) {
                CString strName;
                pMenu->GetMenuString((UINT)pKey->wCmdID, strName, MF_BYCOMMAND);

                // 現在のアクセラレータキー部分を削除
                int     nPos;
                if ((nPos = strName.Find(_T('\t'))) != -1) {
                    strName = strName.Left(nPos);
                }

                if (pAccel != NULL) {
                    // 新しいアクセラレータキー部分を追加
                    strName += _T('\t');
                    if (wModifiers & HOTKEYF_ALT    ) strName += _T("Alt+");
                    if (wModifiers & HOTKEYF_CONTROL) strName += _T("Ctrl+");
                    if (wModifiers & HOTKEYF_SHIFT  ) strName += _T("Shift+");
                    // キー部分を追加
                    if (pAccel->key <= 0x91) {
                        strName += g_sKeyName[pAccel->key];
                    } else {
                        strName += _T("?");
                    }
                }

                // メニュー文字列を更新
                pMenu->ModifyMenu((UINT)pKey->wCmdID, MF_BYCOMMAND, pKey->wCmdID, (LPCTSTR)strName);
            }
        }
    }

    // アクセラレータテーブルの登録
    m_hAccel = CreateAcceleratorTable(m_accelTable, nCount);

    pMenu = pMainWnd ? pMainWnd->GetMenu() : NULL;
    if (pMenu != NULL) {
        CMenu* pTeikeiMenu = NULL;
        int nCount = pMenu->GetMenuItemCount();
        int j; for (int j=0;j<nCount;j++) {
            CString strTitle;
            pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
            if (strTitle == _T("編集(&E)")) {
                pMenu = pMenu->GetSubMenu(j);
                break;
            }
        }
        nCount = pMenu->GetMenuItemCount();
        for (j=0;j<nCount;j++) {
            CString strTitle;
            pMenu->GetMenuString(j, strTitle, MF_BYPOSITION);
            if (strTitle == _T("定型文貼り付け")) {
                pTeikeiMenu = pMenu->GetSubMenu(j);
                break;
            }
        }
        for (int k = 0; k < 3 && pTeikeiMenu; k++) {
            CMenu* pSubMenu = pTeikeiMenu->GetSubMenu(0);
            pTeikeiMenu->RemoveMenu(0, MF_BYPOSITION);
            pTeikeiMenu->AppendMenu(MF_POPUP , (UINT_PTR)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
        }
    }
}

//---------------------------------------------------------------------------
// func:    レジストリからWinampのパスを取り出す
//---------------------------------------------------------------------------
void CSuperTagEditorApp::GetWinampPath(void)
{
    HKEY    hKey;
    CString strKeyString = _T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Winamp.exe");

    g_sOptWinAmpPath = _T("");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyString, 0, KEY_EXECUTE, &hKey) == ERROR_SUCCESS) {
        DWORD   dwValueType;
        DWORD   dwValueSize = FILENAME_MAX*sizeof(TCHAR);
        TCHAR   sValue[FILENAME_MAX];

        /* レジストリ値の取得 */
        RegQueryValueEx(hKey, _T(""), NULL, &dwValueType, (LPBYTE)&sValue[0], &dwValueSize);

        /* 結果の表示 */
        if (dwValueType == REG_SZ) g_sOptWinAmpPath = sValue;

        /* 終了 */
        RegCloseKey(hKey);
    }
}

// ジャンルリストの設定をレジストリから読みこむ
void CSuperTagEditorApp::ReadGenreList(void)
{
    int nIndex;
    for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
        GENRE_LIST  *pGenre = &g_genreListSCMPX[nIndex];
        CString strName;
        strName.Format(_T("%d"), pGenre->byGenre);
        pGenre->bAddList = m_IniFile.ReadInt(sSectionGenreList, strName, 1) ? true : false;
    }
    TCHAR buf[2048];
    int i;
    for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
        CString     strSectionName;
        strSectionName.Format(_T("haseta\\%s\\%d"), sSectionUserGenreList, i);
        g_genreListUSER[i].bAddList = m_IniFile.ReadInt(strSectionName, sKeyUserGenreAddList, 0) == 0 ? false : true;
        g_genreListUSER[i].byGenre = (BYTE)m_IniFile.ReadInt(strSectionName, sKeyUserGenreNo, 255);
        g_genreListUSER[i].sName = m_IniFile.ReadStr(strSectionName, sKeyUserGenreName, NULL, buf, _countof(buf));
        if (g_genreListUSER[i].sName.GetLength() == 0) {
            g_genreListUSER[i].bUse = false;
        } else {
            g_genreListUSER[i].bUse = true;
        }
    }
    //FreeProfile();
}

// ジャンルリストの設定をレジストリに書き出す
void CSuperTagEditorApp::WriteGenreList(void)
{
    int nIndex;
    for (nIndex = 0; g_genreListSCMPX[nIndex].sName != NULL; nIndex++) {
        GENRE_LIST  *pGenre = &g_genreListSCMPX[nIndex];
        CString strName;
        strName.Format(_T("%d"), pGenre->byGenre);
        m_IniFile.WriteInt(sSectionGenreList, strName, pGenre->bAddList ? 1 : 0);
    }
    if (g_genreListUSER != NULL) {
        int i; for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
            CString     strSectionName;
            strSectionName.Format(_T("haseta\\%s\\%d"), sSectionUserGenreList, i);
            m_IniFile.WriteInt(strSectionName, sKeyUserGenreAddList, g_genreListUSER[i].bAddList == true ? 1 : 0);
            m_IniFile.WriteInt(strSectionName, sKeyUserGenreNo, g_genreListUSER[i].byGenre);
            m_IniFile.WriteStr(strSectionName, sKeyUserGenreName, g_genreListUSER[i].sName);
        }
    }
    m_IniFile.Flush();
}

void CSuperTagEditorApp::LoadStdProfileSettings(UINT nMaxMRU)  /* StartInaction 053 */
{
    ASSERT_VALID(this);
    ASSERT(m_pRecentFileList == NULL);

    if (nMaxMRU != 0) {
        m_pRecentFileList = new CMyRecentFileList(0, _T("haseta\\MRU"), _T("MRUFolder%d"), nMaxMRU, 60);
        m_pRecentFileList->ReadList();
    }
}

void CSuperTagEditorApp::OnUpdateFileMruFile(CCmdUI* pCmdUI) /* StartInaction 053 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    m_pRecentFileList->UpdateMenu(pCmdUI);
}

void CSuperTagEditorApp::AddToRecentFileList(LPCTSTR lpszPathName) /* StartInaction 053 */
{
    CString strPath = lpszPathName; /* WildCherry2 079 */
    if (IsFolderName(lpszPathName) == true) {
        strPath.Delete(strPath.GetLength()-1);
    }
    m_pRecentFileList->Add(strPath);
}

BOOL CSuperTagEditorApp::OnOpenRecentFile(UINT nID) /* BeachMonster 104 */
{
    // 他のやり方が判らなかったので、OnOpenRecentFile()をオーバーライドして自前で処理した
    int nIndex = nID - ID_FILE_MRU_FILE1;
    ASSERT((*m_pRecentFileList)[nIndex].GetLength() != 0);

    TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
            (LPCTSTR)(*m_pRecentFileList)[nIndex]);

    POSITION pos = GetFirstDocTemplatePosition();
    if (pos != NULL) {
        CDocTemplate* pDocTemp = GetNextDocTemplate(pos);
        if (pDocTemp != NULL) {
            POSITION docPos = pDocTemp->GetFirstDocPosition();
            if (docPos != NULL) {
                CSuperTagEditorDoc* pDoc = (CSuperTagEditorDoc*)pDocTemp->GetNextDoc(docPos);
                if (pDoc != NULL) {
                    pDoc->OnOpenDocument((*m_pRecentFileList)[nIndex]);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

void CSuperTagEditorApp::OnFavoriteFolder(CString& strFolder)
{
    POSITION pos = GetFirstDocTemplatePosition();
    if (pos != NULL) {
        CDocTemplate* pDocTemp = GetNextDocTemplate(pos);
        if (pDocTemp != NULL) {
            POSITION docPos = pDocTemp->GetFirstDocPosition();
            if (docPos != NULL) {
                CSuperTagEditorDoc* pDoc = (CSuperTagEditorDoc*)pDocTemp->GetNextDoc(docPos);
                if (pDoc != NULL) {
                    pDoc->OnOpenDocument(strFolder);
                    return;
                }
            }
        }
    }
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder01(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[0].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder01() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[0]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder02(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[1].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder02() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[1]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder03(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[2].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder03() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[2]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder04(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[3].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder04() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[3]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder05(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[4].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder05() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[4]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder06(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[5].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder06() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[5]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder07(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[6].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder07() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[6]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder08(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[7].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder08() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[7]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder09(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[8].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder09() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[8]);
}

void CSuperTagEditorApp::OnUpdateFavoriteFolder10(CCmdUI* pCmdUI) /* RockDance 124 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(!g_strFavorite[9].IsEmpty());
}

void CSuperTagEditorApp::OnFavoriteFolder10() /* RockDance 124 */
{
    OnFavoriteFolder(g_strFavorite[9]);
}

void CSuperTagEditorApp::ReadFixedWordList()
{
    TCHAR   drive[_MAX_DRIVE];
    TCHAR   dir[_MAX_DIR];
    TCHAR   buff[_MAX_PATH] = {0};

    //自己アプリのパス所得（大小文字識別付き）
    GetModuleFileName(m_hInstance, buff, _MAX_PATH);
    _tsplitpath_s(buff, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
    _tmakepath_s(buff, drive, dir, _T("FixedWordList"), _T("ini"));

    g_arFixedWords.RemoveAll();
    TRY {
        CFile   file;
        if (file.Open(buff, CFile::modeRead|CFile::shareDenyNone)) {
            CArchive    ar(&file, CArchive::load);
            CString strLine;
            while(ar.ReadString(strLine)) {
                g_arFixedWords.Add(strLine);
            }
        }
    }
    CATCH( CFileException, e) {
        CString str;
        str.Format(_T("%s の読み込みに失敗しました"), buff);
        MessageBox(NULL, str, _T("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
    }
    END_CATCH
}

void CSuperTagEditorApp::WriteFixedWordList()
{
    TCHAR   drive[_MAX_DRIVE];
    TCHAR   dir[_MAX_DIR];
    TCHAR   buff[_MAX_PATH] = {0};

    //自己アプリのパス所得（大小文字識別付き）
    GetModuleFileName(m_hInstance, buff, _MAX_PATH);
    _tsplitpath_s(buff, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
    _tmakepath_s(buff, drive, dir, _T("FixedWordList"), _T("ini"));

    TRY {
        CFile   file;
        if (file.Open(buff, CFile::modeCreate|CFile::modeWrite)) {
            CArchive    ar(&file, CArchive::store);
            CString strLine;
            for (int i=0;i<g_arFixedWords.GetSize();i++) {
                ar.WriteString(g_arFixedWords.GetAt(i) + _T("\n"));
            }
        }
    }
    CATCH( CFileException, e) {
        CString str;
        str.Format(_T("%s の書き込みに失敗しました"), buff);
        MessageBox(NULL, str, _T("ファイルエラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
    }
    END_CATCH
}

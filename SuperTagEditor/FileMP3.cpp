// ======================================================
// 概  要    ： ＭＰ３ファイル管理クラス
// ターゲット： Windows95/98/NT
// 処理系    ： Visual C++ Ver 6.0
// 作成者    ： MERCURY
// 作成日    ： 00/01/18(火)
// 著作権表記： Copyright(C) 2000 MERCURY.
// ======================================================



// ======================================
// =====   条件コンパイルフラグ     =====
// ======================================



// ======================================
// =====   インクルードファイル     =====
// ======================================
#include "stdafx.h"
#include "FileMP3.h"
#include "supertageditor.h"
#include "DlgFileOverWrite.h"
#include "DlgFileNameChange.h"


#define USE_SCMPX_GENRE_ANIMEJ g_genreListSCMPX[255].bAddList /* BeachMonster 099 */

// ======================================
// =====           定  数           =====
// ======================================



// ======================================
// =====           マクロ           =====
// ======================================



// ======================================
// =====       typedef／enum        =====
// ======================================



// ======================================
// =====       構造体／共用体       =====
// ======================================



// ======================================
// =====         const 変数         =====
// ======================================



// ======================================
// =====        extern 変数         =====
// ======================================
int  g_nWriteTagProcFlag = 0;

// ======================================
// =====        static 変数         =====
// ======================================
// ジャンル名称テーブル
GENRE_LIST  g_genreListSCMPX[] = {
    {true,   0, _T("Blues")},                   // 0(ID3 Tag互換)
    {true,   1, _T("Classic Rock")},            // 1(ID3 Tag互換)
    {true,   2, _T("Country")},                 // 2(ID3 Tag互換)
    {true,   3, _T("Dance")},                   // 3(ID3 Tag互換)
    {true,   4, _T("Disco")},                   // 4(ID3 Tag互換)
    {true,   5, _T("Funk")},                    // 5(ID3 Tag互換)
    {true,   6, _T("Grunge")},                  // 6(ID3 Tag互換)
    {true,   7, _T("Hip-Hop")},                 // 7(ID3 Tag互換)
    {true,   8, _T("Jazz")},                    // 8(ID3 Tag互換)
    {true,   9, _T("Metal")},                   // 9(ID3 Tag互換)
    {true,  10, _T("New Age")},                 // 10(ID3 Tag互換)
    {true,  11, _T("Oldies")},                  // 11(ID3 Tag互換)
    {true,  12, _T("Other")},                   // 12(ID3 Tag互換)
    {true,  13, _T("Pop")},                     // 13(ID3 Tag互換)
    {true,  14, _T("R&B")},                     // 14(ID3 Tag互換)
    {true,  15, _T("Rap")},                     // 15(ID3 Tag互換)
    {true,  16, _T("Reggae")},                  // 16(ID3 Tag互換)
    {true,  17, _T("Rock")},                    // 17(ID3 Tag互換)
    {true,  18, _T("Techno")},                  // 18(ID3 Tag互換)
    {true,  19, _T("Industrial")},              // 19(ID3 Tag互換)
    {true,  20, _T("Alternative")},             // 20(ID3 Tag互換)
    {true,  21, _T("Ska")},                     // 21(ID3 Tag互換)
    {true,  22, _T("Death Metal")},             // 22(ID3 Tag互換)
    {true,  23, _T("Pranks")},                  // 23(ID3 Tag互換)
    {true,  24, _T("Soundtrack")},              // 24(ID3 Tag互換)
    {true,  25, _T("Euro-Techno")},             // 25(ID3 Tag互換)
    {true,  26, _T("Ambient")},                 // 26(ID3 Tag互換)
    {true,  27, _T("Trip-Hop")},                // 27(ID3 Tag互換)
    {true,  28, _T("Vocal")},                   // 28(ID3 Tag互換)
    {true,  29, _T("Jazz+Funk")},               // 29(ID3 Tag互換)
    {true,  30, _T("Fusion")},                  // 30(ID3 Tag互換)
    {true,  31, _T("Trance")},                  // 31(ID3 Tag互換)
    {true,  32, _T("Classical")},               // 32(ID3 Tag互換)
    {true,  33, _T("Instrumental")},            // 33(ID3 Tag互換)
    {true,  34, _T("Acid")},                    // 34(ID3 Tag互換)
    {true,  35, _T("House")},                   // 35(ID3 Tag互換)
    {true,  36, _T("Game")},                    // 36(ID3 Tag互換)
    {true,  37, _T("Sound Clip")},              // 37(ID3 Tag互換)
    {true,  38, _T("Gospel")},                  // 38(ID3 Tag互換)
    {true,  39, _T("Noise")},                   // 39(ID3 Tag互換)
    {true,  40, _T("AlternRock")},              // 40(ID3 Tag互換)
    {true,  41, _T("Bass")},                    // 41(ID3 Tag互換)
    {true,  42, _T("Soul")},                    // 42(ID3 Tag互換)
    {true,  43, _T("Punk")},                    // 43(ID3 Tag互換)
    {true,  44, _T("Space")},                   // 44(ID3 Tag互換)
    {true,  45, _T("Meditative")},              // 45(ID3 Tag互換)
    {true,  46, _T("Instrumental Pop")},        // 46(ID3 Tag互換)
    {true,  47, _T("Instrumental Rock")},       // 47(ID3 Tag互換)
    {true,  48, _T("Ethnic")},                  // 48(ID3 Tag互換)
    {true,  49, _T("Gothic")},                  // 49(ID3 Tag互換)
    {true,  50, _T("Darkwave")},                // 50(ID3 Tag互換)
    {true,  51, _T("Techno-Industrial")},       // 51(ID3 Tag互換)
    {true,  52, _T("Electronic")},              // 52(ID3 Tag互換)
    {true,  53, _T("Pop-Folk")},                // 53(ID3 Tag互換)
    {true,  54, _T("Eurodance")},               // 54(ID3 Tag互換)
    {true,  55, _T("Dream")},                   // 55(ID3 Tag互換)
    {true,  56, _T("Southern Rock")},           // 56(ID3 Tag互換)
    {true,  57, _T("Comedy")},                  // 57(ID3 Tag互換)
    {true,  58, _T("Cult")},                    // 58(ID3 Tag互換)
    {true,  59, _T("Gangsta")},                 // 59(ID3 Tag互換)
    {true,  60, _T("Top 40")},                  // 60(ID3 Tag互換)
    {true,  61, _T("Christian Rap")},           // 61(ID3 Tag互換)
    {true,  62, _T("Pop/Funk")},                // 62(ID3 Tag互換)
    {true,  63, _T("Jungle")},                  // 63(ID3 Tag互換)
    {true,  64, _T("Native American")},         // 64(ID3 Tag互換)
    {true,  65, _T("Cabaret")},                 // 65(ID3 Tag互換)
    {true,  66, _T("New Wave")},                // 66(ID3 Tag互換)
    {true,  67, _T("Psychedelic")},             // 67(ID3 Tag互換)
    {true,  68, _T("Rave")},                    // 68(ID3 Tag互換)
    {true,  69, _T("Showtunes")},               // 69(ID3 Tag互換)
    {true,  70, _T("Trailer")},                 // 70(ID3 Tag互換)
    {true,  71, _T("Lo-Fi")},                   // 71(ID3 Tag互換)
    {true,  72, _T("Tribal")},                  // 72(ID3 Tag互換)
    {true,  73, _T("Acid Punk")},               // 73(ID3 Tag互換)
    {true,  74, _T("Acid Jazz")},               // 74(ID3 Tag互換)
    {true,  75, _T("Polka")},                   // 75(ID3 Tag互換)
    {true,  76, _T("Retro")},                   // 76(ID3 Tag互換)
    {true,  77, _T("Musical")},                 // 77(ID3 Tag互換)
    {true,  78, _T("Rock & Roll")},             // 78(ID3 Tag互換)
    {true,  79, _T("Hard Rock")},               // 79(ID3 Tag互換)
    {true,  80, _T("Folk")},                    // 80(ID3 Tag互換)
    {true,  81, _T("Folk/Rock")},               // 81(ID3 Tag互換)
    {true,  82, _T("National Folk")},           // 82(ID3 Tag互換)
    {true,  83, _T("Swing")},                   // 83(ID3 Tag互換)
    {true,  84, _T("Fast-Fusion")},             // 84(Winamp互換)
    {true,  85, _T("Bebob")},                   // 85(Winamp互換)
    {true,  86, _T("Latin")},                   // 86(Winamp互換)
    {true,  87, _T("Revival")},                 // 87(Winamp互換)
    {true,  88, _T("Celtic")},                  // 88(Winamp互換)
    {true,  89, _T("Bluegrass")},               // 89(Winamp互換)
    {true,  90, _T("Avantgarde")},              // 90(Winamp互換)
    {true,  91, _T("Gothic Rock")},             // 91(Winamp互換)
    {true,  92, _T("Progressive Rock")},        // 92(Winamp互換)
    {true,  93, _T("Psychedelic Rock")},        // 93(Winamp互換)
    {true,  94, _T("Symphonic Rock")},          // 94(Winamp互換)
    {true,  95, _T("Slow Rock")},               // 95(Winamp互換)
    {true,  96, _T("Big Band")},                // 96(Winamp互換)
    {true,  97, _T("Chorus")},                  // 97(Winamp互換)
    {true,  98, _T("Easy Listening")},          // 98(Winamp互換)
    {true,  99, _T("Acoustic")},                // 99(Winamp互換)
    {true, 100, _T("Humour")},                  // 100(Winamp互換)
    {true, 101, _T("Speech")},                  // 101(Winamp互換)
    {true, 102, _T("Chanson")},                 // 102(Winamp互換)
    {true, 103, _T("Opera")},                   // 103(Winamp互換)
    {true, 104, _T("Chamber Music")},           // 104(Winamp互換)
    {true, 105, _T("Sonata")},                  // 105(Winamp互換)
    {true, 106, _T("Symphony")},                // 106(Winamp互換)
    {true, 107, _T("Booty Bass")},              // 107(Winamp互換)
    {true, 108, _T("Primus")},                  // 108(Winamp互換)
    {true, 109, _T("Porn Groove")},             // 109(Winamp互換)
    {true, 110, _T("Satire")},                  // 110(Winamp互換)
    {true, 111, _T("Slow Jam")},                // 111(Winamp互換)
    {true, 112, _T("Club")},                    // 112(Winamp互換)
    {true, 113, _T("Tango")},                   // 113(Winamp互換)
    {true, 114, _T("Samba")},                   // 114(Winamp互換)
    {true, 115, _T("Folklore")},                // 115(Winamp互換)
    {true, 116, _T("Ballad")},                  // 116(Winamp互換)
    {true, 117, _T("Power Ballad")},            // 117(Winamp互換)
    {true, 118, _T("Phythmic Soul")},           // 118(Winamp互換)
    {true, 119, _T("Freestyle")},               // 119(Winamp互換)
    {true, 120, _T("Duet")},                    // 120(Winamp互換)
    {true, 121, _T("Punk Rock")},               // 121(Winamp互換)
    {true, 122, _T("Drum Solo")},               // 122(Winamp互換)
    {true, 123, _T("A Cappella")},              // 123(Winamp互換)
    {true, 124, _T("Euro-House")},              // 124(Winamp互換)
    {true, 125, _T("Dance Hall")},              // 125(Winamp互換)
    {true, 126, _T("Goa")},                     // 126(Winamp互換)
    {true, 127, _T("Drum & Bass")},             // 127(Winamp互換)
    {true, 128, _T("Club-House")},              // 128(Winamp互換)
    {true, 129, _T("Hardcore")},                // 129(Winamp互換)
    {true, 130, _T("Terror")},                  // 130(Winamp互換)
    {true, 131, _T("Indie")},                   // 131(Winamp互換)
    {true, 132, _T("BritPop")},                 // 132(Winamp互換)
    {true, 133, _T("Negerpunk")},               // 133(Winamp互換)
    {true, 134, _T("Polsk Punk")},              // 134(Winamp互換)
    {true, 135, _T("Beat")},                    // 135(Winamp互換)
    {true, 136, _T("Christian Gangsta Rap")},   // 136(Winamp互換)
    {true, 137, _T("Heavy Metal")},             // 137(Winamp互換)
    {true, 138, _T("Black Metal")},             // 138(Winamp互換)
    {true, 139, _T("Crossover")},               // 139(Winamp互換)
    {true, 140, _T("Contemporary Christian")},  // 140(Winamp互換)
    {true, 141, _T("Christian Rock")},          // 141(Winamp互換)
    {true, 142, _T("Merengue")},                // 142(Winamp互換)
    {true, 143, _T("Salsa")},                   // 143(Winamp互換)
    {true, 144, _T("Thrash Metal")},            // 144(Winamp互換)
    {true, 145, _T("Anime")},                   // 145(Winamp互換)
    {true, 146, _T("JPop")},                    // 146(Winamp互換)
    {true, 147, _T("Synthpop")},                // 147(Winamp互換)
    {false, 148, _T("未定義(148)")},            // 148
    {false, 148, _T("未定義(148)")},            // 148
    {false, 149, _T("未定義(149)")},            // 149
    {false, 150, _T("未定義(150)")},            // 150
    {false, 151, _T("未定義(151)")},            // 151
    {false, 152, _T("未定義(152)")},            // 152
    {false, 153, _T("未定義(153)")},            // 153
    {false, 154, _T("未定義(154)")},            // 154
    {false, 155, _T("未定義(155)")},            // 155
    {false, 156, _T("未定義(156)")},            // 156
    {false, 157, _T("未定義(157)")},            // 157
    {false, 158, _T("未定義(158)")},            // 158
    {false, 159, _T("未定義(159)")},            // 159
    {false, 160, _T("未定義(160)")},            // 160
    {false, 161, _T("未定義(161)")},            // 161
    {false, 162, _T("未定義(162)")},            // 162
    {false, 163, _T("未定義(163)")},            // 163
    {false, 164, _T("未定義(164)")},            // 164
    {false, 165, _T("未定義(165)")},            // 165
    {false, 166, _T("未定義(166)")},            // 166
    {false, 167, _T("未定義(167)")},            // 167
    {false, 168, _T("未定義(168)")},            // 168
    {false, 169, _T("未定義(169)")},            // 169
    {false, 170, _T("未定義(170)")},            // 170
    {false, 171, _T("未定義(171)")},            // 171
    {false, 172, _T("未定義(172)")},            // 172
    {false, 173, _T("未定義(173)")},            // 173
    {false, 174, _T("未定義(174)")},            // 174
    {false, 175, _T("未定義(175)")},            // 175
    {false, 176, _T("未定義(176)")},            // 176
    {false, 177, _T("未定義(177)")},            // 177
    {false, 178, _T("未定義(178)")},            // 178
    {false, 179, _T("未定義(179)")},            // 179
    {false, 180, _T("未定義(180)")},            // 180
    {false, 181, _T("未定義(181)")},            // 181
    {false, 182, _T("未定義(182)")},            // 182
    {false, 183, _T("未定義(183)")},            // 183
    {false, 184, _T("未定義(184)")},            // 184
    {false, 185, _T("未定義(185)")},            // 185
    {false, 186, _T("未定義(186)")},            // 186
    {false, 187, _T("未定義(187)")},            // 187
    {false, 188, _T("未定義(188)")},            // 188
    {false, 189, _T("未定義(189)")},            // 189
    {false, 190, _T("未定義(190)")},            // 190
    {false, 191, _T("未定義(191)")},            // 191
    {false, 192, _T("未定義(192)")},            // 192
    {false, 193, _T("未定義(193)")},            // 193
    {false, 194, _T("未定義(194)")},            // 194
    {false, 195, _T("未定義(195)")},            // 195
    {false, 196, _T("未定義(196)")},            // 196
    {false, 197, _T("未定義(197)")},            // 197
    {false, 198, _T("未定義(198)")},            // 198
    {false, 199, _T("未定義(199)")},            // 199
    {false, 200, _T("未定義(200)")},            // 200
    {false, 201, _T("未定義(201)")},            // 201
    {false, 202, _T("未定義(202)")},            // 202
    {false, 203, _T("未定義(203)")},            // 203
    {false, 204, _T("未定義(204)")},            // 204
    {false, 205, _T("未定義(205)")},            // 205
    {false, 206, _T("未定義(206)")},            // 206
    {false, 207, _T("未定義(207)")},            // 207
    {false, 208, _T("未定義(208)")},            // 208
    {false, 209, _T("未定義(209)")},            // 209
    {false, 210, _T("未定義(210)")},            // 210
    {false, 211, _T("未定義(211)")},            // 211
    {false, 212, _T("未定義(212)")},            // 212
    {false, 213, _T("未定義(213)")},            // 213
    {false, 214, _T("未定義(214)")},            // 214
    {false, 215, _T("未定義(215)")},            // 215
    {false, 216, _T("未定義(216)")},            // 216
    {false, 217, _T("未定義(217)")},            // 217
    {false, 218, _T("未定義(218)")},            // 218
    {false, 219, _T("未定義(219)")},            // 219
    {false, 220, _T("未定義(220)")},            // 220
    {false, 221, _T("未定義(221)")},            // 221
    {false, 222, _T("未定義(222)")},            // 222
    {false, 223, _T("未定義(223)")},            // 223
    {false, 224, _T("未定義(224)")},            // 224
    {false, 225, _T("未定義(225)")},            // 225
    {false, 226, _T("未定義(226)")},            // 226
    {false, 227, _T("未定義(227)")},            // 227
    {false, 228, _T("未定義(228)")},            // 228
    {false, 229, _T("未定義(229)")},            // 229
    {false, 230, _T("未定義(230)")},            // 230
    {false, 231, _T("未定義(231)")},            // 231
    {false, 232, _T("未定義(232)")},            // 232
    {false, 233, _T("未定義(233)")},            // 233
    {false, 234, _T("未定義(234)")},            // 234
    {false, 235, _T("未定義(235)")},            // 235
    {false, 236, _T("未定義(236)")},            // 236
    {false, 237, _T("未定義(237)")},            // 237
    {false, 238, _T("未定義(238)")},            // 238
    {false, 239, _T("未定義(239)")},            // 239
    {false, 240, _T("未定義(240)")},            // 240
    {false, 241, _T("未定義(241)")},            // 241
    {false, 242, _T("未定義(242)")},            // 242
    {false, 243, _T("未定義(243)")},            // 243
    {false, 244, _T("未定義(244)")},            // 244
    {false, 245, _T("未定義(245)")},            // 245
    {false, 246, _T("未定義(246)")},            // 246
    {false, 247, _T("未定義(247)")},            // 247
    {false, 248, _T("Heavy Rock(J)")},          // 248(SCMPX互換)
    {false, 249, _T("Doom Rock(J)")},           // 249(SCMPX互換)
    {false, 250, _T("J-POP(J)")},               // 250(SCMPX互換)
    {false, 251, _T("Seiyu(J)")},               // 251(SCMPX互換)
    {false, 252, _T("Tecno Ambient(J)")},       // 252(SCMPX互換)
    {false, 253, _T("Moemoe(J)")},              // 253(SCMPX互換)
    {false, 254, _T("Tokusatsu(J)")},           // 254(SCMPX互換)
    {false, 255, _T("Anime(J)")},               // 255(SCMPX互換)
    {false, 255, _T("未指定")},                 // 255
    {false, 255, NULL},                     // 終端コード
};
USER_GENRE_LIST*    g_genreListUSER = NULL;



// ======================================
// =====     関数プロトタイプ       =====
// ======================================


// =============================================
// GetGenreNameSIF
// 概要  : ジャンル番号からジャンル名を取得(SIフィールド用)
// 引数  : byGenre          = ジャンル番号
// 戻り値: const char *     = ジャンル名
// =============================================
const TCHAR *GetGenreNameSIF(BYTE byGenre)
{
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (g_genreListSCMPX[i].byGenre == byGenre) {
            if (byGenre == 0xff) {
                // SCMPX 互換かどうかで文字列を変える
                return(g_genreListSCMPX[i].bAddList ? _T("Anime(J)") : _T(""));
            }
            return(g_genreListSCMPX[i].sName);
        }
    }
    return(_T(""));
}

// =============================================
// GetGenreCode
// 概要  : ジャンル名からジャンル番号を取得
// 引数  : sGenre       = ジャンル名
// 戻り値: int          = ジャンル番号(-1:未対応)
// =============================================
int GetGenreCode(const TCHAR *sGenre)
{
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (_tcsicmp(g_genreListSCMPX[i].sName, sGenre) == 0) {
            return(g_genreListSCMPX[i].byGenre);
        }
    }
    for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
        if (_tcsicmp(g_genreListUSER[i].sName, sGenre) == 0 && g_genreListUSER[i].bUse) {
            return(g_genreListUSER[i].byGenre);
        }
    }
    return(-1);
}

bool IsUserGenre(const TCHAR *sGenre)
{
    if (_tcslen(sGenre) == 0) return false; /* WildCherry2 075 */
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (_tcsicmp(g_genreListSCMPX[i].sName, sGenre) == 0) {
            return false;
        }
    }
    for (i = 0; i < USER_GENRE_LIST_MAX; i++) {

        if (_tcsicmp(g_genreListUSER[i].sName, sGenre) == 0 && g_genreListUSER[i].bUse) {
            return true;
        }
    }
    return true;
}

static  bool MyChangeFileName(const TCHAR *sFileName, const TCHAR *sNewName)
{
    if (MoveFile(sFileName, sNewName) == FALSE) {
        CString strBuffer;
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
                      (LPTSTR)&lpMsgBuf, 0, NULL);
        strBuffer.Format(_T("変更前：%s\n変更後：%s\n\n%s"), sFileName, sNewName, (const TCHAR *)lpMsgBuf);
        LocalFree(lpMsgBuf);
        MessageBox(NULL, strBuffer, _T("ファイル名を変更できませんでした"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return(false);
    }
    return(true);
}

static  bool MyCopyFile(const TCHAR *sFileName, const TCHAR *sNewName, bool bMoveFlag)
{
    BOOL    bResult;

    // コピー／移動を実行
    if (bMoveFlag) {
        // 移動先に同名のファイルがある場合は、予め削除する
        if (SetFileAttributes(sNewName, CFile::normal) == TRUE) {
            if (DeleteFile(sNewName) == FALSE) {
//              CString strBuffer;
//              LPVOID lpMsgBuf;
//              FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
//                            FORMAT_MESSAGE_FROM_SYSTEM |
//                            FORMAT_MESSAGE_IGNORE_INSERTS,
//                            NULL, GetLastError(),
//                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
//                            (LPTSTR)&lpMsgBuf, 0, NULL);
//              strBuffer.Format("ファイル名：%s\n\n%s", sNewName, (const char *)lpMsgBuf);
//              LocalFree(lpMsgBuf);
//              MessageBox(NULL, strBuffer, "ファイルの削除に失敗しました", MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST);
//              return(false);
            }
        }
        bResult = MoveFile(sFileName, sNewName);
    } else {
        bResult = CopyFile(sFileName, sNewName, FALSE);
    }

    // エラー処理
    if (bResult == FALSE) {
        static  const TCHAR *sMessage[][2] = {
            {_T("コピー元：%s\nコピー先：%s\n\n%s"), _T("ファイルのコピーに失敗しました")},
            {_T("移動元：%s\n移動先：%s\n\n%s"), _T("ファイルの移動に失敗しました")},
        };
        CString strBuffer;
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
                      (LPTSTR)&lpMsgBuf, 0, NULL);
        strBuffer.Format(sMessage[bMoveFlag?1:0][0], sFileName, sNewName, (const TCHAR *)lpMsgBuf);
        LocalFree(lpMsgBuf);
        MessageBox(NULL, strBuffer, sMessage[bMoveFlag?1:0][1], MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return(false);
    }
    return(true);
}

static  void ChangeFileAttr(const TCHAR *sFileName, DWORD dwAttr)
{
    DWORD   dwAttrNow;
    if ((dwAttrNow = GetFileAttributes(sFileName)) != (DWORD)-1L) {
        if (dwAttrNow != dwAttr) {
            SetFileAttributes(sFileName, dwAttr);
        }
    }
}

// 最後が '\\' になっているかどうかのチェック関数
bool IsFolderName(const TCHAR *sName)
{
    const TCHAR *path_delimiter = _tcsrchr(sName, _T('\\'));
    return path_delimiter && !path_delimiter[1];
}


// =============================================
// CFileMP3::CFileMP3
// Func  : コンストラクタ
// Input : none
// Output: none
// =============================================
CFileMP3::CFileMP3()
{
    m_fileMP3.fileSpecificInfo = NULL;
    Initialize();
}

// =============================================
// CFileMP3::~CFileMP3
// Func  : デストラクタ
// Input : none
// Output: none
// =============================================
CFileMP3::~CFileMP3()
{
    //if (m_fileMP3.fileOGG != NULL)    delete m_fileMP3.fileOGG; /* Conspiracy 196 */
}

// =============================================
// CFileMP3::InitData
// 概要  : MP3 ファイル情報の構造体を初期化します
// 引数  : fileMP3      = 初期化する構造体
// 戻り値: none
// =============================================
void CFileMP3::InitData(FILE_MP3 *fileMP3)
{
    extern void InitFileSpecificInfo(FILE_MP3* pFileMP3);
    if (fileMP3->fileSpecificInfo != NULL) {
        InitFileSpecificInfo(fileMP3);
        fileMP3->fileSpecificInfo = NULL;
    }

    fileMP3->bModifyFlag    = false;    // 外部から編集されたかどうかのフラグ
    fileMP3->bCheckFlag     = false;    // チェック状態
    fileMP3->lFileSize      = 0L;       // ファイルサイズ
//	fileMP3->time			= CTime(0);	// ファイル更新日
//	fileMP3->timeCreate		= CTime(0);	// ファイル作成日
    fileMP3->time = COleDateTime();	// ファイル更新日 /* STEP 044 */
    fileMP3->timeCreate = COleDateTime();	// ファイル作成日 /* STEP 044 */
#ifdef USE_VBMP3_DLL
    fileMP3->nPlayTime      = 0;        // 演奏時間(単位：秒)
#endif
    fileMP3->nPlayTime      = -1;       // 演奏時間(単位：秒) /* Conspiracy 195 */
    fileMP3->bExistLyricFile = false;   // 歌詞ファイルの存在
    fileMP3->nFormat        = FILE_FORMAT_UNKNOWN;  // ファイル形式：未知の形式
    fileMP3->strFullPathName.Empty();   // ファイル名(フルパス)
    fileMP3->strFileName.Empty();       // ファイル名
    fileMP3->strFilePath.Empty();       // パス名

    // ID3 のクリア
    InitDataID3(fileMP3); /* STEP 029 */

    // SIF のクリア
    InitDataSIF(fileMP3);
}

void CFileMP3::InitDataID3(FILE_MP3 *fileMP3) /* STEP 029 */
{
    fileMP3->strTrackName.Empty();      // トラック名
    fileMP3->strArtistName.Empty();     // アーティスト名
    fileMP3->strAlbumName.Empty();      // アルバム名
    fileMP3->strComment.Empty();        // コメント
    fileMP3->strYear.Empty();           // リリース年号
    fileMP3->byTrackNumber  = 0xff;     // トラック番号(数値編集用)
    fileMP3->strTrackNumber.Empty();    // トラック番号(文字列編集用)
    fileMP3->byGenre        = 0xff;     // ジャンル番号
    fileMP3->strGenre.Empty();          // ジャンル名

}

// =============================================
//  CFileMP3::InitDataSIF
//  概要  : MP3 ファイル情報の構造体を初期化します(SIF関連のみ)
//  引数  : fileMP3     = 初期化する構造体
//  戻り値: なし
// =============================================
void CFileMP3::InitDataSIF(FILE_MP3 *fileMP3)
{
    // RIFF MP3 の SI フィールド
    fileMP3->strTrackNameSI.Empty();    // 曲名
    fileMP3->strArtistNameSI.Empty();   // アーティスト名
    fileMP3->strAlbumNameSI.Empty();    // 製品名
    fileMP3->strCommentSI.Empty();      // コメント文字列
    fileMP3->strYearSI.Empty();         // リリース年号
    fileMP3->strGenreSI.Empty();        // ジャンル名
    fileMP3->strCopyrightSI.Empty();    // 著作権
    fileMP3->strEngineerSI.Empty();     // エンジニア
    fileMP3->strSourceSI.Empty();       // ソース
    fileMP3->strSoftwareSI.Empty();     // ソフトウェア
    fileMP3->strKeywordSI.Empty();      // キーワード
    fileMP3->strTechnicianSI.Empty();   // 技術者
    fileMP3->strLyricSI.Empty();        // 歌詞
    fileMP3->strCommissionSI.Empty();   // コミッション

    if (IsUserGenre(fileMP3->strGenre)) {
        fileMP3->strGenre = GetGenreNameSIF(fileMP3->byGenre);
    }
    fileMP3->strTrackNumber.Empty();
    if (fileMP3->byTrackNumber != 0xFF) {
        fileMP3->strTrackNumber.Format(_T("%d"), fileMP3->byTrackNumber);
    }
    fileMP3->strTrackNumberSI.Empty();
    fileMP3->strTrackTotalSI.Empty();   // トラック数(Kobarin)
    fileMP3->strDiscNumberSI.Empty();
    fileMP3->strDiscTotalSI.Empty();    // ディスク数(Kobarin)
    fileMP3->strWriterSI.Empty();       // 作詞
    fileMP3->strComposerSI.Empty();     // 作曲
    fileMP3->strAlbmArtistSI.Empty();   // Albm.アーティスト
    fileMP3->strOrigArtistSI.Empty();   // Orig.アーティスト
    fileMP3->strURLSI.Empty();          // URL
    fileMP3->strEncodest.Empty();       // エンコードした人
    fileMP3->strOther.Empty();          // その他
    //fileMP3->strAlbumArtist.Empty();	// アルバムアーティスト /* STEP 042 */
    fileMP3->strWriter.Empty();			// 作詞者 /* STEP 043 */

}

// =============================================
// CFileMP3::Initialize
// 概要  : 初期化処理
// 引数  : none
// 戻り値: none
// =============================================
void CFileMP3::Initialize(void)
{
    //m_fileMP3.fileSpecificInfo = NULL;
    InitData(&m_fileMP3);
}

// =============================================
// CFileMP3::DeleteLineEndSpace
// 概要  : 行末のスペースを取り除く
// 引数  : sBuffer      = 文字列
// 戻り値: none
// =============================================
void CFileMP3::DeleteLineEndSpace(TCHAR *sBuffer)
{
#ifndef _UNICODE
    TCHAR *spc = NULL;
    while(*sBuffer){
        if(_istlead(*sBuffer)){
            if(!sBuffer[1]){
                *sBuffer = 0;
                break;
            }
            sBuffer+=2;
            spc = NULL;
            continue;
        }
        if(*sBuffer == _T(' ')){
            if(!spc){
                spc = sBuffer;
            }
        }
        else{
            spc = NULL;
        }
    }
    if(spc){
        *spc = 0;
    }
#else
    size_t len = _tcslen(sBuffer);
    while(len >= 1 && sBuffer[len-1] == _T(' ')) {
        sBuffer[len-1] = 0;
        len--;
    }
#endif
}

// =============================================
//  CFileMP3::Attach
//  概要  : クラスにファイルに関連付けする
//  引数  : sFileName   = ファイル名
//  戻り値: bool        = true:正常終了 / false:エラー
// =============================================
bool CFileMP3::Attach(const TCHAR *sFileName)
{
    Initialize();

    // タイムスタンプを取得
    CFileStatus fileStatus = {0};
    /* STEP 044 */
//    try{//タイムスタンプが異常だと例外が発生するらしいのでごまかす
//
//    if (CFile::GetStatus(sFileName, fileStatus) == FALSE || fileStatus.m_mtime == -1) {
//        fileStatus.m_mtime = CTime(0);
//        fileStatus.m_ctime = CTime(0);
//    }
//
//    }catch(...){
//        fileStatus.m_mtime = CTime(0);
//        fileStatus.m_ctime = CTime(0);
//    }
    CFile::GetStatus(sFileName, fileStatus);
    if (GetFileTimeStamp(sFileName, m_fileMP3.timeCreate, m_fileMP3.time) == false) { /* STEP 044 */
        m_fileMP3.time = COleDateTime();
        m_fileMP3.timeCreate = COleDateTime();
    }
    /* STEP 044 */
//	m_fileMP3.time			= fileStatus.m_mtime;	// ファイル更新日
//	m_fileMP3.timeCreate	= fileStatus.m_ctime;	// ファイル作成日
    m_fileMP3.lFileSize = fileStatus.m_size;	// ファイルサイズ

    // フルパスをパス名とファイル名に分解
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(sFileName, drive, dir, fname, ext);
    m_fileMP3.strFilePath.Format(_T("%s%s"), drive, dir);
    m_fileMP3.strFileName.Format(_T("%s%s"), fname, ext);
    m_fileMP3.strFullPathName = sFileName;

    // ファイル情報の読み込み
    extern bool LoadFile(const TCHAR *sFileName, const TCHAR *sExt, FILE_MP3* pFileMP3);
    if (LoadFile(sFileName, &ext[1], &m_fileMP3) == false) {
        Initialize();
        return(false);
    }

    // 歌詞ファイルの存在をチェック
    if (g_bOptSearchLyricFile) {
        // 歌詞ファイルを検索
        m_fileMP3.bExistLyricFile = SearchLyricsFile(&m_fileMP3).IsEmpty() ? false : true;
    }

    return(true);
}

// =============================================
//  CFileMP3::Detach
//  概要  : クラスとファイルの関連付けを削除する
//  引数  : なし
//  戻り値: なし
// =============================================
void CFileMP3::Detach(void)
{
    Initialize();
}

// =============================================
//  CFileMP3::WriteTag
//  概要  : タグ情報の更新
//  引数  : fileMP3     = ファイル情報
//        : bKeepTimeStamp= タイムスタンプを保持するフラグ
//  戻り値: bool        = true:正常終了 / false:エラー
// =============================================
bool CFileMP3::WriteTag(FILE_MP3 *fileMP3, bool bKeepTimeStamp)
{
    CFileStatus fileStatus;
    bool    bResult = false;

    if (g_nWriteTagProcFlag == 3) {
        return(false);                      // 以降中止
    }

    /* STEP 044 */
//    try{
//        // タイムスタンプを取得
//        if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
//            return(false);
//        }
//        if (fileStatus.m_mtime == -1)   fileStatus.m_mtime = fileStatus.m_ctime; /* 2005.10.11 日時がおかしい場合に落ちないように */
//        if (fileStatus.m_atime == -1)   fileStatus.m_atime = fileStatus.m_mtime; /* 2005.10.11 日時がおかしい場合に落ちないように */
//    }
//    catch (...) {
//        if (fileStatus.m_mtime == -1)   fileStatus.m_mtime = fileStatus.m_ctime;
//        if (fileStatus.m_atime == -1)   fileStatus.m_atime = fileStatus.m_mtime;
//    }
    COleDateTime mtime, ctime;
    GetFileTimeStamp(fileMP3->strFullPathName, ctime, mtime);

    // 読み出し専用ファイルは、上書き確認を行う
    if (fileStatus.m_attribute & CFile::readOnly) {
        if (g_nWriteTagProcFlag != 1) {
            // 上書き確認ダイアログを開く
            CDlgFileOverWrite   dialog;
            dialog.m_strFileName = fileMP3->strFullPathName;
            dialog.m_strSize.Format(_T("%llu byte"), fileStatus.m_size); /* STEP J-h %ld -> %llu */
            /* STEP 044 *///			if (fileStatus.m_mtime.GetTime() < 0) {
            if (mtime.GetStatus() != COleDateTime::DateTimeStatus::valid) {
                dialog.m_strTimeStamp.Format(_T("----/--/-- --:--:--"));
            } else {
                dialog.m_strTimeStamp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
                    /* STEP 044 */			 /*fileStatus.m_mtime.GetYear(),
                                             fileStatus.m_mtime.GetMonth(),
                                             fileStatus.m_mtime.GetDay(),
                                             fileStatus.m_mtime.GetHour(),
                                             fileStatus.m_mtime.GetMinute(),
                                             fileStatus.m_mtime.GetSecond());*/
                                            mtime.GetYear(),
                                            mtime.GetMonth(),
                                            mtime.GetDay(),
                                            mtime.GetHour(),
                                            mtime.GetMinute(),
                                            mtime.GetSecond());
            }
            dialog.DoModal();
            g_nWriteTagProcFlag = dialog.m_nResult;
            switch(dialog.m_nResult) {
            case 0:             // このファイルを上書き
            case 1:             // 以降全て上書き
                break;
            case 2:             // スキップ
            case 3:             // 中止
            default:
                return(false);
            }
        }

        // 読み込み専用属性を解除
        SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
        // 変更後に属性を戻させるために bKeepTimeStamp を true にする
        bKeepTimeStamp = true;
    }

    extern bool SaveFile(FILE_MP3* pFileMP3);
    if ((bResult = SaveFile(fileMP3)) == true) {
        // ファイル名変換
        //bResult = ConvFileName(fileMP3);
    }

    // タイムスタンプをファイル更新前に戻す
    if (bResult) {
        if (bKeepTimeStamp) {
            /* STEP 044 *//*
            if (fileMP3->time.GetTime() != -1) {
                // ファイル更新時間を設定
                fileStatus.m_mtime = fileMP3->time;
                if (g_bOptSyncCreateTime) fileStatus.m_ctime = fileMP3->time;
            }
            TRY {
                CFile::SetStatus(fileMP3->strFullPathName, fileStatus);
            }
            CATCH(CFileException, e) {
                CString strMsg;
                strMsg.Format(_T("%s がオープンできませんでした"), (LPCWSTR)fileMP3->strFullPathName);
                MessageBox(NULL, strMsg, _T("タイムスタンプの更新エラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
                bResult = false;
            }
            END_CATCH
            */
            if (fileMP3->time.GetStatus() == COleDateTime::DateTimeStatus::valid) {
                // ファイル更新時間を設定
                mtime = fileMP3->time;
                if (g_bOptSyncCreateTime) ctime = fileMP3->time;
            }
            if (SetFileTimeStamp(fileMP3->strFullPathName, ctime, mtime) != true) {
                CString	strMsg;
                strMsg.Format(_T("%s がオープンできませんでした"), (LPCWSTR)fileMP3->strFullPathName);
                MessageBox(NULL, strMsg, _T("タイムスタンプの更新エラー"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
                bResult = false;
            }
        } else {
            // 更新後のタイムスタンプを取得
            /* STEP 044 *//*
            try {
                if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) != FALSE) {
                    fileMP3->time = fileStatus.m_mtime;
                }
            }
            catch(...){
                fileMP3->time = fileStatus.m_mtime;
            }
            */
            if (GetFileTimeStamp(fileMP3->strFullPathName, ctime, mtime) != false) {
                fileMP3->time = mtime;
            }
        }
    }

    return(bResult);
}

// =============================================
// CFileMP3::SearchFileReent
// 概要  : ファイル検索：検索処理
// 引数  : sDir         = 検索するディレクトリ
//       : sTargetName  = 検索するファイル名
// 戻り値: CString   = 発見したファイルのフルパス(IsEmpty():未発見)
// =============================================
CString CFileMP3::SearchFileReent(const TCHAR *sDir, const TCHAR *sTargetName)
{
    HANDLE  hFindFile;
    WIN32_FIND_DATA data;
    CString strTargetFile;

    // 同一ディレクトリの全てのファイルを検索
    // 検索ファイルのパターンを設定
    CString strCurFile;
    strCurFile.Format(_T("%s%s"), sDir, sTargetName);
    if ((hFindFile = FindFirstFile(strCurFile, &data)) != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                // ファイル発見処理
                strCurFile.Format(_T("%s%s"), sDir, data.cFileName);
                return(strCurFile);
            }
        } while(FindNextFile(hFindFile, &data));
    }
    if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);

    // サブディレクトリの検索
    if (g_bOptSearchLyricsSubDir == true) {
        strCurFile.Format(_T("%s*.*"), sDir);
        if ((hFindFile = FindFirstFile(strCurFile, &data)) != INVALID_HANDLE_VALUE) {
            do {
                if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                    if (_tcscmp(data.cFileName, _T(".") ) != 0      // カレントディレクトリ以外
                    &&  _tcscmp(data.cFileName, _T("..")) != 0      // 親ディレクトリ以外
                    && (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 /* STEP 032 */) { // 隠しフォルダ以外
                        // サブディレクトリ検索処理
                        strCurFile.Format(_T("%s%s\\"), sDir, data.cFileName);
                        strTargetFile = SearchFileReent(strCurFile, sTargetName);
                        if (strTargetFile.IsEmpty() == false) {
                            break;              // ファイル発見
                        }
                    }
                }
            } while(FindNextFile(hFindFile, &data));
        }
        if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);
    }

    return(strTargetFile);
}

// =============================================
//  CFileMP3::SearchLyricsFile
//  概要  : 歌詞ファイルの検索
//        : 優先順位：MP3と同じフォルダ→歌詞フォルダ→歌詞フォルダのサブディレクトリ
//  引数  : fileMP3     = ファイル情報
//  戻り値: CString  = 歌詞ファイル名(見つからなかった場合は空)
// =============================================
CString CFileMP3::SearchLyricsFile(FILE_MP3 *fileMP3)
{
    TCHAR   sTextName[FILENAME_MAX+1];

    // まずは MP3 ファイルと同じフォルダをチェック
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME];
    _tsplitpath_s(fileMP3->strFullPathName, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, 0);
    // .lrc を検索
    _tmakepath_s(sTextName, drive, dir, fname, _T(".lrc"));
    if (GetFileAttributes(sTextName) != 0xFFFFFFFF) {
        // ファイルを発見
        return(CString(sTextName));
    }
    // .txt を検索
    _tmakepath_s(sTextName, drive, dir, fname, _T(".txt"));
    if (GetFileAttributes(sTextName) != 0xFFFFFFFF) {
        // ファイルを発見
        return(CString(sTextName));
    }

    // 歌詞ファイル名作成
    if (g_bOptSetLyricsDir == true && g_strOptLyricsPath.IsEmpty() == false) {
        // 指定されたフォルダ
        TCHAR   sLyricsPath[FILENAME_MAX+1];
        CString strTarget;
        _tsplitpath_s(fileMP3->strFullPathName, NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0);
        _tcsncpy_s(sLyricsPath, FILENAME_MAX, g_strOptLyricsPath, _TRUNCATE);
        if (IsFolderName(sLyricsPath) == false) {
            _tcsncat_s(sLyricsPath, FILENAME_MAX+1, _T("\\"), _TRUNCATE);
        }
        // .lrc を検索
        strTarget.Format(_T("%s.lrc"), fname);
        strTarget = SearchFileReent(sLyricsPath, strTarget);
        if (strTarget.IsEmpty() == false
        &&  GetFileAttributes(strTarget) != 0xFFFFFFFF) {
            // ファイルを発見
            return(strTarget);
        }
        // .txt を検索
        strTarget.Format(_T("%s.txt"), fname);
        strTarget = SearchFileReent(sLyricsPath, strTarget);
        if (strTarget.IsEmpty() == false
        &&  GetFileAttributes(strTarget) != 0xFFFFFFFF) {
            // ファイルを発見
            return(strTarget);
        }
    }

    // 見つからなかった
    return(CString(""));
}

// =============================================
//  CFileMP3::ConvFileName
//  概要  : ファイル名の変更
//        : ※新しいファイル名は fileMP3->strFileName を使用
//  引数  : fileMP3     = ファイル情報
//  戻り値: bool        = true:正常終了 / false:エラー
// =============================================
bool CFileMP3::ConvFileName(FILE_MP3 *fileMP3)
{
    // ファイル名を分解
    CString strFileName;
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
    strFileName.Format(_T("%s%s"), fname, ext);

    if (_tcscmp(strFileName, fileMP3->strFileName) != 0) {
        // ファイル名が変更されている
        if (g_bConfFileNameMaxCheck) {
            if (fileMP3->strFileName.GetLength() > g_nConfFileNameMaxChar) {
                // ファイル名文字数オーバー
                _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
                CDlgFileNameChange dialog;
                int ext_len = _tcslen(ext);
                if(ext_len > g_nConfFileNameMaxChar){
                    ext_len = g_nConfFileNameMaxChar;
                }
                dialog.m_nMaxChar = g_nConfFileNameMaxChar - ext_len;
                dialog.m_strFileName = fname;
                dialog.m_strOrgFileName = fname;
                //dialog.m_strMsg.Format("ファイル名を %d 文字以内に変更してください", dialog.m_nMaxChar);
                dialog.m_strMsgFormat = _T("ファイル名を %d(%d) 文字以内に変更してください");
                if (dialog.DoModal() == IDOK) {
                    fileMP3->strFileName = dialog.m_strFileName + ext;
                    _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
                }
            }
        }
        TCHAR   sNewName[FILENAME_MAX+1];
        _tmakepath_s(sNewName, drive, dir, NULL, NULL);
        _tcsncat_s(sNewName, fileMP3->strFileName, _TRUNCATE);
        if (MyChangeFileName(fileMP3->strFullPathName, sNewName) == false) {
            return(false);
        }

        // 同名のテキストファイルのファイル名も変換する
        if (g_bOptChangeTextFile) {
            CString strLyricsFile;
            // 歌詞ファイルを検索
            strLyricsFile = SearchLyricsFile(fileMP3);
            if (strLyricsFile.IsEmpty() == false) {
                // 発見した歌詞ファイルのパスを分解
                _tsplitpath_s(strLyricsFile, drive, dir, fname, ext);

                // 歌詞ファイルをリネームする
                TCHAR   sNewText[FILENAME_MAX+1];
                _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, FILENAME_MAX+1, NULL, 0);
                _tmakepath_s(sNewText, drive, dir, fname, ext);
                if (MyChangeFileName(strLyricsFile, sNewText) == false) {
                    // return(false);
                }
            }
        }

        // ファイル名を更新する
        fileMP3->strFullPathName = sNewName;
    }

    return(true);
}

// =============================================
//  CFileMP3::CopyFile
//  概要  : ファイルのコピー／移動
//  引数  : fileMP3     = ファイル情報
//        : sNewDir     = 移動先フォルダ名
//        : bMoveFlag   = フラグ(true:移動/false:コピー)
//  戻り値: bool        = true:正常終了 / false:エラー
// =============================================
bool CFileMP3::CopyFile(FILE_MP3 *fileMP3, const TCHAR *sNewDir, bool bMoveFlag)
{
    // ファイル名を分解
    CString strFileName;
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
    strFileName.Format(_T("%s%s"), fname, ext);

    if (_tcsicmp(fileMP3->strFilePath, sNewDir) != 0) {
        // ファイル名が変更されている
        CString strNewName;
        strNewName.Format(_T("%s%s"), sNewDir, (LPCWSTR)strFileName);
        if (MyCopyFile(fileMP3->strFullPathName, strNewName, bMoveFlag) == false) {
            // ファイルのコピー／移動に失敗!!
            return(false);
        }

        // 移動の場合は、歌詞ファイルも移動する
        if (g_bSyncLyricsFileMove && bMoveFlag == true) {
            TCHAR   sLyricsFile[FILENAME_MAX+1];
            // MP3 ファイルと同じフォルダにある歌詞ファイルを検索
            _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
            // .lrc => .txt の順で検索
            int i; for (i = 0; i < 2; i++) {
                LPTSTR  sLyricsExt = (i == 0) ? _T(".lrc") : _T(".txt");
                _tmakepath_s(sLyricsFile, drive, dir, fname, sLyricsExt);
                if (GetFileAttributes(sLyricsFile) != 0xFFFFFFFF) {
                    // ファイルを発見：歌詞ファイルを移動する
                    CString strNewName;
                    strNewName.Format(_T("%s%s%s"), sNewDir, fname, sLyricsExt);
                    if (MyCopyFile(sLyricsFile, strNewName, true) == false) {
                        // 失敗!!
                        // return(false);
                    }
                    break;
                }
            }
        }

        // 管理情報のファイル名を更新する
        if (bMoveFlag) {
            fileMP3->strFilePath = sNewDir;
            fileMP3->strFullPathName = strNewName;
        }
    }

    return(true);
}

CString CFileMP3::GetIntTrackNo(const CString &no)
{
    //トラック番号
    if (IsNumeric(no)) {
        return no;
    } else {
        /* BeachMonster 094 */
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Left(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                //return "255";
            } else {
                CString track;
                track.Format(_T("%d"), _ttoi(no.Left(index)));
                return track;
            }
        }
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Right(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                return "";
            } else {
                CString track;
                track.Format(_T("%d"), _ttoi(no.Right(index)));
                return track;
            }
        }
    }
}

CString CFileMP3::GetIntDiscNo(const CString &no)
{
    //トラック番号
    if (IsNumeric(no)) {
        return no;
    } else {
        /* BeachMonster 094 */
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Left(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                //return "255";
            } else {
                CString Disc;
                Disc.Format(_T("%d"), _ttoi(no.Left(index)));
                return Disc;
            }
        }
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Right(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                return "";
            } else {
                CString Disc;
                Disc.Format(_T("%d"), _ttoi(no.Right(index)));
                return Disc;
            }
        }
    }
}


bool CFileMP3::IsNumeric(const CString &str)
{
    if (str.IsEmpty()) {
        return TRUE/*FALSE*/; /* WildCherry 063 */
    }
    for (int i=0;i<str.GetLength();i++) {
        if (_istlead(str[i])) {
            // 漢字リーディングバイト
            i++;
            return FALSE;
        } else {
            if (!_istdigit(str[i])) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/* STEP 044 */
bool CFileMP3::GetFileTimeStamp(/*const char* */ LPCTSTR sFileName, COleDateTime& ctime, COleDateTime& mtime)
{
    HANDLE     hFile;
    FILETIME   creationTime, lastWriteTime, locTime;
    SYSTEMTIME sysTime;

    // ファイルハンドルの取得
    hFile = CreateFile(sFileName,
        GENERIC_READ/* | GENERIC_WRITE*/,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        if (GetFileTime(hFile, &creationTime, NULL, &lastWriteTime) == 0) {	// UTC世界標準時
            CloseHandle(hFile);
            ctime.SetStatus(COleDateTime::DateTimeStatus::invalid);
            mtime.SetStatus(COleDateTime::DateTimeStatus::invalid);
            return false;
        }

        FileTimeToLocalFileTime(&creationTime, &locTime);			 // ローカルタイム(日本時間)に
        FileTimeToSystemTime(&locTime, &sysTime);					 // システムタイムに
        ctime.SetDateTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

        FileTimeToLocalFileTime(&lastWriteTime, &locTime);
        FileTimeToSystemTime(&locTime, &sysTime);
        mtime.SetDateTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

        CloseHandle(hFile);
        return true;
    }
    ctime.SetStatus(COleDateTime::DateTimeStatus::invalid);
    mtime.SetStatus(COleDateTime::DateTimeStatus::invalid);
    return false;
}

/* STEP 044 */
bool CFileMP3::SetFileTimeStamp(/* const char* */ LPCTSTR sFileName, COleDateTime& ctime, COleDateTime& mtime)
{
    HANDLE     hFile;
    FILETIME   creationTime, lastWriteTime, locTime;
    SYSTEMTIME sysTime;

    // ファイルハンドルの取得
    hFile = CreateFile(sFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        sysTime.wYear = ctime.GetYear();
        sysTime.wMonth = ctime.GetMonth();
        sysTime.wDay = ctime.GetDay();
        sysTime.wHour = ctime.GetHour();
        sysTime.wMinute = ctime.GetMinute();
        sysTime.wSecond = ctime.GetSecond();
        sysTime.wMilliseconds = 0;
        SystemTimeToFileTime(&sysTime, &locTime);
        LocalFileTimeToFileTime(&locTime, &creationTime);

        sysTime.wYear = mtime.GetYear();
        sysTime.wMonth = mtime.GetMonth();
        sysTime.wDay = mtime.GetDay();
        sysTime.wHour = mtime.GetHour();
        sysTime.wMinute = mtime.GetMinute();
        sysTime.wSecond = mtime.GetSecond();
        sysTime.wMilliseconds = 0;
        SystemTimeToFileTime(&sysTime, &locTime);
        LocalFileTimeToFileTime(&locTime, &lastWriteTime);

        if (SetFileTime(hFile, &creationTime, NULL, &lastWriteTime) == 0) {    // タイムスタンプの修正
            CloseHandle(hFile);
            return false;
        }

        CloseHandle(hFile);
        return true;
    }
    return false;
}

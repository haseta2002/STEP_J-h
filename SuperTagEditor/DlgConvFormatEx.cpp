// DlgConvFormatEx.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "supertageditor.h"
#include "DlgNameChange.h"
#include "DlgConvFormatEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgConvFormatEx, COptionPage)

CDlgConvFormatEx::CDlgConvFormatEx() : COptionPage(CDlgConvFormatEx::IDD)
{
    //{{AFX_DATA_INIT(CDlgConvFormatEx)
    m_nAddNumber = 1;
    m_nInitNumber = 1;
    m_nMinColumn = 1;
    m_nFormatType = 0;
    //}}AFX_DATA_INIT
}

CDlgConvFormatEx::~CDlgConvFormatEx()
{
}

void CDlgConvFormatEx::DoDataExchange(CDataExchange* pDX)
{
    COptionPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgConvFormatEx)
    DDX_Control(pDX, IDC_CH_SPACE_INIT, m_btSpaceInitNumber);
    DDX_Control(pDX, IDC_LIST_FORMAT_TYPE, m_listFormatType);
    DDX_Control(pDX, IDC_EDIT_FORMAT, m_editFormat);
    DDX_Text(pDX, IDC_ED_ADD_NUMBER, m_nAddNumber);
    DDV_MinMaxInt(pDX, m_nAddNumber, 0, 999999);
    DDX_Text(pDX, IDC_ED_INIT_NUMBER, m_nInitNumber);
    DDV_MinMaxInt(pDX, m_nInitNumber, 0, 999999);
    DDX_Text(pDX, IDC_ED_MIN_COLUMN, m_nMinColumn);
    DDV_MinMaxUInt(pDX, m_nMinColumn, 1, 9);
    DDX_CBIndex(pDX, IDC_LIST_FORMAT_TYPE, m_nFormatType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConvFormatEx, COptionPage)
    //{{AFX_MSG_MAP(CDlgConvFormatEx)
    ON_BN_CLICKED(IDC_BT_ALBUM_NAME, OnBtAlbumName)
    ON_BN_CLICKED(IDC_BT_ARTIST_NAME, OnBtArtistName)
    ON_BN_CLICKED(IDC_BT_COMMENT, OnBtComment)
    ON_BN_CLICKED(IDC_BT_FILE_NAME, OnBtFileName)
    ON_BN_CLICKED(IDC_BT_FIX_STRING, OnBtFixString)
    ON_BN_CLICKED(IDC_BT_NUMBER, OnBtNumber)
    ON_BN_CLICKED(IDC_BT_NUMBER2, OnBtNumber2)
    ON_BN_CLICKED(IDC_BT_RELEASE_YEAR, OnBtReleaseYear)
    ON_BN_CLICKED(IDC_BT_TRACK_NAME, OnBtTrackName)
    ON_BN_CLICKED(IDC_BT_TRACK_NUMBER, OnBtTrackNumber)
    ON_BN_CLICKED(IDC_BT_TRACK_NUMBER2, OnBtTrackNumber2)
    ON_BN_CLICKED(IDC_BT_NAME_CHANGE, OnBtNameChange)
    ON_CBN_SELCHANGE(IDC_LIST_FORMAT_TYPE, OnSelChangeListFormatType)
    ON_BN_CLICKED(IDC_BT_DISC_NUMBER, OnBtDiscNumber)
    ON_BN_CLICKED(IDC_BT_DISC_NUMBER2, OnBtDiscNumber2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConvFormatEx メッセージ ハンドラ

void CDlgConvFormatEx::OutputFormatString(const TCHAR *sText)
{
    m_editFormat.ReplaceSel(sText, FALSE);
    m_editFormat.SetFocus();
}

void CDlgConvFormatEx::OnBtAlbumName()
{
    OutputFormatString(_T("%ALBUM_NAME%"));
}

void CDlgConvFormatEx::OnBtArtistName()
{
    OutputFormatString(_T("%ARTIST_NAME%"));
}

void CDlgConvFormatEx::OnBtComment()
{
    OutputFormatString(_T("%COMMENT%"));
}

void CDlgConvFormatEx::OnBtFileName()
{
    OutputFormatString(_T("%FILE_NAME%"));
}

void CDlgConvFormatEx::OnBtFixString()
{
    OutputFormatString(_T("%STRING%"));
}

void CDlgConvFormatEx::OnBtNumber()
{
    OutputFormatString(_T("%NUMBER%"));
}

void CDlgConvFormatEx::OnBtNumber2()
{
    OutputFormatString(_T("%NUMBER2%"));
}

void CDlgConvFormatEx::OnBtReleaseYear()
{
    OutputFormatString(_T("%RELEASE_YEAR%"));
}

void CDlgConvFormatEx::OnBtTrackName()
{
    OutputFormatString(_T("%TRACK_NAME%"));
}

void CDlgConvFormatEx::OnBtTrackNumber()
{
    OutputFormatString(_T("%TRACK_NUMBER%"));
}

void CDlgConvFormatEx::OnBtTrackNumber2()
{
    OutputFormatString(_T("%TRACK_NUMBER2%"));
}

void CDlgConvFormatEx::OnBtDiscNumber()
{
    OutputFormatString(_T("%DISC_NUMBER%"));
}

void CDlgConvFormatEx::OnBtDiscNumber2()
{
    OutputFormatString(_T("%DISC_NUMBER2%"));
}

void CDlgConvFormatEx::OnBtNameChange()
{
    CDlgNameChange    dialog;

    dialog.m_strName = m_userFormatEx[m_nFormatType].strName;
    if (dialog.DoModal() == IDOK) {
        // 新しい名称に変更
        m_userFormatEx[m_nFormatType].strName = dialog.m_strName;

        // リスト更新
        OnSelChangeListFormatType();
        m_listFormatType.ResetContent();
        int i; for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
            CString strName;
            strName.Format(_T("%d: %s"), i+1, (LPCWSTR)m_userFormatEx[i].strName);
            m_listFormatType.AddString(strName);
        }
        // 選択しなおす
        m_listFormatType.SetCurSel(m_nFormatType);
    }
}

void CDlgConvFormatEx::OnSelChangeListFormatType()
{
    // 現在編集中の内容を反映させる
    m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_userFormatEx[m_nFormatType].bSpaceInitNumber = m_btSpaceInitNumber.GetCheck() ? true : false;
    m_userFormatEx[m_nFormatType].nInitNumber = GetDlgItemInt(IDC_ED_INIT_NUMBER, NULL, FALSE);
    m_userFormatEx[m_nFormatType].nAddNumber = GetDlgItemInt(IDC_ED_ADD_NUMBER, NULL, FALSE);
    m_userFormatEx[m_nFormatType].nColumnCount = GetDlgItemInt(IDC_ED_MIN_COLUMN, NULL, FALSE);

    m_nFormatType = m_listFormatType.GetCurSel();
    m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_btSpaceInitNumber.SetCheck(m_userFormatEx[m_nFormatType].bSpaceInitNumber ? TRUE : FALSE);
    SetDlgItemInt(IDC_ED_INIT_NUMBER, m_userFormatEx[m_nFormatType].nInitNumber, FALSE);
    SetDlgItemInt(IDC_ED_ADD_NUMBER, m_userFormatEx[m_nFormatType].nAddNumber, FALSE);
    SetDlgItemInt(IDC_ED_MIN_COLUMN, m_userFormatEx[m_nFormatType].nColumnCount, FALSE);

    m_nInitNumber = m_userFormatEx[m_nFormatType].nInitNumber;
    m_nAddNumber = m_userFormatEx[m_nFormatType].nAddNumber;
    m_nMinColumn = m_userFormatEx[m_nFormatType].nColumnCount;
}

BOOL CDlgConvFormatEx::OnInitDialog()
{
    COptionPage::OnInitDialog();

    int     i;
    m_listFormatType.ResetContent();
    for (i = 0; i < USER_CONV_FORMAT_EX_MAX; i++) {
        CString strName;
        strName.Format(_T("%d: %s"), i+1, (LPCWSTR)m_userFormatEx[i].strName);
        m_listFormatType.AddString(strName);
    }

    m_listFormatType.SetCurSel(m_nFormatType);
    m_editFormat.SetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_btSpaceInitNumber.SetCheck(m_userFormatEx[m_nFormatType].bSpaceInitNumber ? TRUE : FALSE);
    SetDlgItemInt(IDC_ED_INIT_NUMBER, m_userFormatEx[m_nFormatType].nInitNumber, FALSE);
    SetDlgItemInt(IDC_ED_ADD_NUMBER, m_userFormatEx[m_nFormatType].nAddNumber, FALSE);
    SetDlgItemInt(IDC_ED_MIN_COLUMN, m_userFormatEx[m_nFormatType].nColumnCount, FALSE);

    m_nInitNumber = m_userFormatEx[m_nFormatType].nInitNumber;
    m_nAddNumber = m_userFormatEx[m_nFormatType].nAddNumber;
    m_nMinColumn = m_userFormatEx[m_nFormatType].nColumnCount;

    return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
                  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgConvFormatEx::OnOK()
{
    if (!::IsWindow(m_hWnd)) return;
    m_editFormat.GetWindowText(m_userFormatEx[m_nFormatType].strFormat);
    m_userFormatEx[m_nFormatType].bSpaceInitNumber = m_btSpaceInitNumber.GetCheck() ? true : false;
    m_userFormatEx[m_nFormatType].nInitNumber = GetDlgItemInt(IDC_ED_INIT_NUMBER, NULL, FALSE);
    m_userFormatEx[m_nFormatType].nAddNumber = GetDlgItemInt(IDC_ED_ADD_NUMBER, NULL, FALSE);
    m_userFormatEx[m_nFormatType].nColumnCount = GetDlgItemInt(IDC_ED_MIN_COLUMN, NULL, FALSE);

    COptionPage::OnOK();
}

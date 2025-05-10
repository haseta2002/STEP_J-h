/* STEP_J-h 004 */
#include "stdafx.h"
#include "CIFileDialogFolderSelector.h"
#include <strsafe.h>

CIFileDialogFolderSelector::CIFileDialogFolderSelector(bool enableSubDirButton, bool searchSubDirState)
    : m_bEnableSubDirButton(enableSubDirButton), m_bSearchSubDirState(searchSubDirState) {
    _tcscpy_s(m_szInitialFolder, _T(""));
    _tcscpy_s(m_szCheckBoxTitle, _T("サブディレクトリを検索"));
}

CIFileDialogFolderSelector::~CIFileDialogFolderSelector() {
    // デストラクタ
}

void CIFileDialogFolderSelector::SetEnableSubDirButton(bool bFlag) {
    m_bEnableSubDirButton = bFlag;
}

bool CIFileDialogFolderSelector::GetEnableSubDirButton() const {
    return m_bEnableSubDirButton;
}

void CIFileDialogFolderSelector::SetSearchSubDirState(bool bFlag) {
    m_bSearchSubDirState = bFlag;
}

bool CIFileDialogFolderSelector::GetSearchSubDirState() const {
    return m_bEnableSubDirButton ? m_bSearchSubDirState : false;
}

void CIFileDialogFolderSelector::SetCheckBoxTitle(const TCHAR* title) {
    _tcsncpy_s(m_szCheckBoxTitle, title, _TRUNCATE);
}

const TCHAR* CIFileDialogFolderSelector::GetCheckBoxTitle() const {
    return m_szCheckBoxTitle;
}

bool CIFileDialogFolderSelector::Exec(TCHAR* sLocal, int size) {
    if (!sLocal || size <= 0) {
        return false;
    }

    // IFileDialog インターフェースを作成
    IFileDialog* pFileDialog = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (FAILED(hr)) {
        return false;
    }

    // フォルダ選択モードを設定
    DWORD dwOptions;
    hr = pFileDialog->GetOptions(&dwOptions);
    if (SUCCEEDED(hr)) {
        pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);
    }

    // 初期フォルダを設定
    if (_tcslen(sLocal) > 0) {
        IShellItem* pInitialFolder = nullptr;
        hr = SHCreateItemFromParsingName(sLocal, nullptr, IID_PPV_ARGS(&pInitialFolder));
        if (SUCCEEDED(hr)) {
            pFileDialog->SetFolder(pInitialFolder);
            pInitialFolder->Release();
        }
    }

    // ダイアログのタイトルを設定
    pFileDialog->SetTitle(m_szCheckBoxTitle);

    // イベントハンドラを設定
    IFileDialogCustomize* pCustomize = nullptr;
    hr = pFileDialog->QueryInterface(IID_PPV_ARGS(&pCustomize));
    if (SUCCEEDED(hr)) {
        if (m_bEnableSubDirButton) {
            pCustomize->AddCheckButton(1, m_szCheckBoxTitle, m_bSearchSubDirState);
        }
        pCustomize->Release();
    }

    // ダイアログを表示
    hr = pFileDialog->Show(nullptr);
    if (SUCCEEDED(hr)) {
        if (m_bEnableSubDirButton) {
            hr = pFileDialog->QueryInterface(IID_PPV_ARGS(&pCustomize));
            if (SUCCEEDED(hr)) {
                BOOL bChecked = FALSE;
                pCustomize->GetCheckButtonState(1, &bChecked);
                m_bSearchSubDirState = (bChecked == TRUE);
                pCustomize->Release();
            }
        }

        // 選択されたフォルダを取得
        IShellItem* pItem = nullptr;
        hr = pFileDialog->GetResult(&pItem);
        if (SUCCEEDED(hr)) {
            PWSTR pszFolderPath = nullptr;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
            if (SUCCEEDED(hr)) {
                // フォルダパスをコピー
                StringCchCopy(sLocal, size, pszFolderPath);
                CoTaskMemFree(pszFolderPath);
                pItem->Release();
                pFileDialog->Release();
                return true;
            }
            pItem->Release();
        }
    }

    pFileDialog->Release();
    return false;
}

/* STEP_J-h 004 */
/*
* SHBrowseForFolder -> IFileDialog
* エクスプローラーのネットワークに表示されない共有フォルダを選択できないため
* (手入力でアクセス可能だがツリービューからアクセスできない場合)
* IFileDialogを利用したクラスを追加
* (https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog)
*
* ----- https://learn.microsoft.com/ja-jp/windows/win32/api/shlobj_core/nf-shlobj_core-shbrowseforfoldera
* Windows Vista 以降では、SHBrowseForFolder 関数ではなく、FOS_PICKFOLDERS オプション IFileDialog を使用することをお勧めします。
* これは、フォルダーの選択モードで [ファイルを開く] ダイアログを使用し、推奨される実装です。
* -----
*/
#ifndef __CI_FILE_DIALOG_FOLDER_SELECTOR_H__
#define __CI_FILE_DIALOG_FOLDER_SELECTOR_H__

#include <windows.h>
#include <shobjidl.h>
#include <tchar.h>

class CIFileDialogFolderSelector {
public:
    CIFileDialogFolderSelector(bool enableSubDirButton = false, bool searchSubDirState = true);
    virtual ~CIFileDialogFolderSelector();

    void SetEnableSubDirButton(bool bFlag);
    bool GetEnableSubDirButton() const;
    void SetSearchSubDirState(bool bFlag);
    bool GetSearchSubDirState() const;
    bool Exec(TCHAR* sLocal, int size);
    void SetCheckBoxTitle(const TCHAR* title);
    const TCHAR* GetCheckBoxTitle() const;

private:
    TCHAR m_szInitialFolder[MAX_PATH];
    TCHAR m_szCheckBoxTitle[256];
    bool m_bEnableSubDirButton;
    bool m_bSearchSubDirState;
};

#endif // __CI_FILE_DIALOG_FOLDER_SELECTOR_H__

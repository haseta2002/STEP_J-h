#include "stdafx.h"
#include "FileAAC.h"
#include "Tag_Mp4.h"

bool LoadFileAAC(FILE_INFO *pFile)
{
    CTag_Mp4 TagMp4;
    TagMp4.Load(GetFullPath(pFile));
    SetTrackNameSI(pFile, TagMp4.GetMetadata_Name());
    SetArtistNameSI(pFile, TagMp4.GetMetadata_Artist());
    SetComposerSI(pFile, TagMp4.GetMetadata_Composer());
    SetCommentSI(pFile, TagMp4.GetMetadata_Comment());
    SetSoftwareSI(pFile, TagMp4.GetMetadata_Tool());
    SetYearSI(pFile, TagMp4.GetMetadata_Year());
    SetAlbumNameSI(pFile, TagMp4.GetMetadata_Album());
    SetAlbumArtistSI(pFile, TagMp4.GetMetadata_AlbumArtist());
    SetGenreSI(pFile, TagMp4.GetMetadata_Genre());
    SetKeywordSI(pFile, TagMp4.GetMetadata_Group());
    SetCopyrightSI(pFile, TagMp4.GetMetadata_Copyright());

    TCHAR buf[256];
    int tracknumber = TagMp4.GetMetadata_Track1();
    int tracktotal  = TagMp4.GetMetadata_Track2();
    int discnumber =  TagMp4.GetMetadata_Disc1();
    int disctotal = TagMp4.GetMetadata_Disc2();
    if(tracknumber > 0){
        _sntprintf_s(buf, _TRUNCATE, _T("%d"), tracknumber);
        SetTrackNumberSI(pFile, buf);
    }
    else{
        SetTrackNumberSI(pFile, _T(""));
    }
    if(tracktotal > 0){
        _sntprintf_s(buf, _TRUNCATE, _T("%d"), tracktotal);
        SetTrackTotalSI(pFile, buf);
    }
    else{
        SetTrackTotalSI(pFile, _T(""));
    }
    if(discnumber > 0){
        _sntprintf_s(buf, _TRUNCATE, _T("%d"), discnumber);
        SetDiscNumberSI(pFile, buf);
    }
    else{
        SetDiscNumberSI(pFile, _T(""));
    }
    if(disctotal > 0){
        _sntprintf_s(buf, _TRUNCATE, _T("%d"), disctotal);
        SetDiscTotalSI(pFile, buf);
    }
    else{
        SetDiscTotalSI(pFile, _T(""));
    }
    CString strAudio = TagMp4.GetAudioInfoString();
    //"format, length secs, kbps, Hz" のようになっている
    //例："MPEG4-AAC LC, 123.456 secs, 128 kbps, 44100 Hz"
    //演奏時間=length部
    //フォーマット="format, kbps, Hz" のように分割して表示
    TCHAR szAudio[256];
    TCHAR szFormat[256];
    _tcsncpy_s(szAudio, strAudio, _TRUNCATE);
    TCHAR *length = _tcschr(szAudio, _T(','));
    if(length){
        *length++ = 0;
        _tcsncpy_s(szFormat, szAudio, _TRUNCATE);
        TCHAR *sec = NULL;
        double dSec = _tcstod(length, &sec);
        SetPlayTime(pFile, dSec);
        TCHAR *kbps = _tcschr(length, _T(','));
        if(kbps){
            _tcsncat_s(szFormat, kbps, _TRUNCATE);
        }
    }
    else{
        _tcsncpy_s(szFormat, szAudio, _TRUNCATE);
    }
    SetAudioFormat(pFile, szFormat);

    int compilation = TagMp4.GetMetadata_Compilation();
    if (compilation >= 0) {
        _sntprintf_s(buf, _TRUNCATE, _T("%d"),compilation);
        SetCompilationSI(pFile, buf);
    }
    else {
        SetCompilationSI(pFile, _T(""));
    }

    return true;
}

bool WriteFileAAC(FILE_INFO *pFile)
{
    CTag_Mp4 TagMp4;
    if(TagMp4.Load(GetFullPath(pFile)) != 0){
        return false;
    }

    TagMp4.SetMetadata_Name(GetTrackNameSI(pFile));
    TagMp4.SetMetadata_Artist(GetArtistNameSI(pFile));
    TagMp4.SetMetadata_Composer(GetComposerSI(pFile));
    TagMp4.SetMetadata_Comment(GetCommentSI(pFile));
    TagMp4.SetMetadata_Tool(GetSoftwareSI(pFile));
    TagMp4.SetMetadata_Album(GetAlbumNameSI(pFile));
    TagMp4.SetMetadata_AlbumArtist(GetAlbumArtistSI(pFile));
    TagMp4.SetMetadata_Genre(GetGenreSI(pFile));
    TagMp4.SetMetadata_Year(GetYearSI(pFile));
    TagMp4.SetMetadata_Group(GetKeywordSI(pFile));
    TagMp4.SetMetadata_Copyright(GetCopyrightSI(pFile));

    CString strTrackNumber = GetTrackNumberSI(pFile);
    CString strTrackTotal = GetTrackTotalSI(pFile);
    CString strDiscNumber = GetDiscNumberSI(pFile);
    CString strDiscTotal = GetDiscTotalSI(pFile);

    int tracknumber = strTrackNumber.IsEmpty() ? -1 : _ttoi(strTrackNumber);
    int tracktotal = strTrackTotal.IsEmpty() ? -1 : _ttoi(strTrackTotal);
    int discnumber = strDiscNumber.IsEmpty() ? -1 : _ttoi(strDiscNumber);
    int disctotal = strDiscTotal.IsEmpty() ? -1 : _ttoi(strDiscTotal);

    TagMp4.SetMetadata_Track1(tracknumber);
    TagMp4.SetMetadata_Track2(tracktotal);
    TagMp4.SetMetadata_Disc1(discnumber);
    TagMp4.SetMetadata_Disc2(disctotal);

    CString strCompilation = GetCompilationSI(pFile);
    int compilation = strCompilation.IsEmpty() ? -1 : _ttoi(strCompilation);
    TagMp4.SetMetadata_Compilation(compilation);

    TagMp4.Save(GetFullPath(pFile));
    return true;
}

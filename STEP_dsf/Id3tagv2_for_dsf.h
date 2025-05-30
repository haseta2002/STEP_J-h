// Id3tagv2.h: CId3tagv2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)
#define AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory.h>

typedef enum
{
    FILETYPE_MP3 = 0,            // 0:MP3/TTA形式
    FILETYPE_DSF,                // 1:DSF形式
    FILETYPEMAX
}FILETYPE;

class CId3Frame
{
public:
    CId3Frame()
    {
        m_data = NULL;
        m_dwId = 0;
        m_dwSize = 0;
        m_wFlags = 0;
    };
    virtual ~CId3Frame()
    {
        Release();
    };
    void Release()
    {
        if(m_data)
        {
            free(m_data);
        }
        m_data = NULL;
        m_dwId = 0;
        m_dwSize = 0;
        m_wFlags = 0;
    }
    CId3Frame(const CId3Frame &obj) //コピーコンストラクタ
    {
        m_data = (unsigned char *)malloc(obj.m_dwSize);
        memcpy(m_data,obj.m_data,obj.m_dwSize);
        m_dwId = obj.m_dwId;
        m_dwSize = obj.m_dwSize;
        m_wFlags = obj.m_wFlags;
    };
    DWORD LoadFrame2_4(const unsigned char *pData,DWORD dwSize);
    DWORD LoadFrame2_3(const unsigned char *pData,DWORD dwSize);
    DWORD LoadFrame2_2(const unsigned char *pData,DWORD dwSize);
    DWORD GetId() const {return m_dwId;};
    void SetId(DWORD id){m_dwId = id;};
    DWORD GetSize() const {return m_dwSize;};
    void SetSize(DWORD size){m_dwSize = size;};
    WORD GetFlags() const {return m_wFlags;};
    void SetFlags(WORD flags){m_wFlags = flags;};
    unsigned char *GetData(){return m_data;};
    const unsigned char *GetData() const {return m_data;};
    void SetData(const unsigned char *data,DWORD size)
    {
        if(m_data)
        {
            free(m_data);
        }
        m_data = (unsigned char *)malloc(size);
        if(!m_data)
        {
            return; //メモリを確保できなかった
        }
        memcpy(m_data,data,size);
        m_dwSize = size;
    };
    DWORD GetDataOffset() const;
    bool IsTextFrame() const {return (m_dwId & 0xff) == 'T';};
    bool IsNumericFrame() const;

    enum Flags {
        MASK_FRAME_STATUS       = 0xff00,    // Mask for frame status flags
        MASK_FRAME_FORMAT       = 0x00ff,    // Mask for frame format flags
        FLAG_TAG_ALT_PRESERVE   = 0x4000,    // Tag alter preservation
        FLAG_FILE_ALT_PRESERVE  = 0x2000,    // File alter preservation
        FLAG_READ_ONLY          = 0x1000,    // Read only
        FLAG_GROUP              = 0x0040,    // Grouping identity
        FLAG_COMPRESS           = 0x0008,    // Compression
        FLAG_ENCRYPT            = 0x0004,    // Encryption
        FLAG_UNSYNC             = 0x0002,    // Unsynchronisation
        FLAG_DATA_LEN           = 0x0001,    // Data length indicator
    };

private:
    DWORD LoadApicFrame(const unsigned char *pData, DWORD dwSize, WORD wVer);

    void operator=(const CId3Frame &){};    // 代入演算子

    unsigned char   *m_data;
    DWORD    m_dwId;
    DWORD    m_dwSize;
    WORD    m_wFlags;
};

#pragma warning(disable:4786)
#include <map>

class CId3tagv2
{
public:
#pragma pack(1)
    typedef struct _ID3HEAD
    {
        char id3des[3];
        char ver[2];
        unsigned char flag;
        unsigned char size[4];
    }ID3HEAD;
#pragma pack()
    enum HeaderFlags {
        HDR_FLAG_UNSYNC         = 0x80,     // Unsynchronisation
        HDR_FLAG_EXT_HEADER     = 0x40,     // Extended header
        HDR_FLAG_EXPERIMENTAL   = 0x20,     // Experimental indicator
        HDR_FLAG_FOOTER         = 0x10,     // Footer present
    };
//    CId3tagv2(WORD defaultVersion = 0x0300/* ID3v2.3 = 0x0300/ID3v2.4 = 0x0400*/);
    CId3tagv2();
    virtual ~CId3tagv2();
    void SetEncDefault(LPCTSTR szDefaultEnc){m_strDefaultEnc = szDefaultEnc;};
    BOOL IsEnable(){return m_bEnable;};
//    void SetDefaultId3v2Version(DWORD version){m_wDefaultId3TagVersion = (WORD )version;};/* ID3v2.3 = 0x0300/ID3v2.4 = 0x0400*/
    void SetVer(WORD ver){m_wVer = ver;};
    WORD GetVer(){return m_wVer;};

    enum CharEncoding {
        ID3V2CHARENCODING_UNSPECIFIED = -1,
        ID3V2CHARENCODING_ISO_8859_1 = 0,
        ID3V2CHARENCODING_UTF_16 = 1,
        ID3V2CHARENCODING_UTF_16BE = 2,
        ID3V2CHARENCODING_UTF_8 = 3
    };
    void SetCharEncoding(CharEncoding encoding)
    {
        // エンコード指定$2/$3が使えるのはv2.4以降
        if(m_wVer < 0x0400)
        {
            if(    (encoding != ID3V2CHARENCODING_ISO_8859_1) &&
                (encoding != ID3V2CHARENCODING_UTF_16) )
            {
                // UTF-16に自動設定
                encoding = ID3V2CHARENCODING_UTF_16;
            }
        }
        m_encoding = encoding;
    };
    CharEncoding GetCharEncoding(){return m_encoding;};

    void SetUnSynchronization(BOOL bEnable){m_bUnSynchronization = bEnable;};
    BOOL GetUnSynchronization(){return m_bUnSynchronization;};

    CString GetTitle() { return GetId3String("TIT2"); };
    void SetTitle(LPCTSTR title) { SetId3String("TIT2",title); };
    CString GetTrackNo() { return GetId3String("TRCK"); };
    void SetTrackNo(LPCTSTR szTrackNo) { SetId3String("TRCK",szTrackNo); };
    CString GetDiscNo() { return GetId3String("TPOS"); };
    void SetDiscNo(LPCTSTR szDiscNo) { SetId3String("TPOS",szDiscNo); };
    CString GetArtist() { return GetId3String("TPE1"); };
    void SetArtist(LPCTSTR artist) { SetId3String("TPE1",artist); };
    CString GetAlbumArtist() { return GetId3String("TPE2"); };
    void SetAlbumArtist(LPCTSTR albumartist) { SetId3String("TPE2",albumartist); };
    CString GetAlbum() { return GetId3String("TALB"); };
    void SetAlbum(LPCTSTR album) { SetId3String("TALB",album); };
    CString GetYear();
    void SetYear(LPCTSTR year);
    CString GetGenre();
    void SetGenre(LPCTSTR szGenre);
    CString GetComment() { return GetId3String("COMM"); };
    void SetComment(LPCTSTR comment) { SetId3String("COMM",comment); };
    CString GetComposer() { return GetId3String("TCOM"); };
    void SetComposer(LPCTSTR composer) { SetId3String("TCOM",composer); };
    CString GetOrigArtist() { return GetId3String("TOPE"); };
    void SetOrigArtist(LPCTSTR origArtist) { SetId3String("TOPE",origArtist); };
    CString GetCopyright() { return GetId3String("TCOP"); };
    void SetCopyright(LPCTSTR copyright) { SetId3String("TCOP",copyright); };
    CString GetUrl() { return GetId3String("WXXX"); };
    void SetUrl(LPCTSTR url) { SetId3String("WXXX",url); };
    CString GetEncoder() { return GetId3String("TSSE"); };
    void SetEncoder(LPCTSTR encoder) { SetId3String("TSSE",encoder); };
    CString GetEncodedBy() { return GetId3String("TENC"); };
    void SetEncodedBy(LPCTSTR encoder) { SetId3String("TENC",encoder); };
    //added by Kobarin
    CString GetWriter(){return GetId3String("TEXT");}
    void SetWriter(LPCTSTR writer){SetId3String("TEXT", writer);}
    CString GetEngineer() { return GetId3String("TPUB"); }
    void SetEngineer(LPCTSTR engineer) { SetId3String("TPUB", engineer); }
    /* STEP 049 */
    CString GetCompilation() { return GetId3String("TCMP"); }
    void SetCompilation(LPCTSTR compilation) { SetId3String("TCMP", compilation); };

    //
    DWORD Load(LPCTSTR szFileName);
    DWORD Save(LPCTSTR szFileName);
    DWORD DelTag(LPCTSTR szFileName);
    DWORD MakeTag(LPCTSTR szFileName);
    static BOOL IsTagValid(const ID3HEAD *head) {
        return memcmp(head->id3des,"ID3",3) == 0;
    };

    static inline DWORD ExtractV2Size(const unsigned char size[4]) {
        return (((DWORD )(size[0]&0x7f)<<21) | ((DWORD )(size[1]&0x7f)<<14) | ((DWORD )(size[2]&0x7f)<<7) | (DWORD )(size[3]&0x7f));
    };
    static DWORD DecodeUnSynchronization(unsigned char *data,DWORD dwSize);

private:
    static DWORD EncodeUnSynchronization(const unsigned char *srcData,DWORD dwSize,unsigned char *dstData);
    static void MakeV2Size(DWORD dwSize,unsigned char size[4]);
    CString GetId3String(const char szId[]);
    void SetId3String(const char szId[],LPCTSTR szString,LPCTSTR szDescription = NULL);
    DWORD GetTotalFrameSize();
//    void _SetStringEncoding(int encoding);
    static void v23IDtov22ID(const char *v23ID,char *v22ID);
    static CString ReadEncodedTextString(unsigned char encoding, const unsigned char *data, DWORD dataize, DWORD *pdwReadSize);
    static CharEncoding GetFrameEncoding(const CId3Frame &frame);
    static DWORD ConvertApicToV22(const unsigned char *v23, DWORD dwSize, unsigned char *v22);

    void Release();
    BOOL m_bEnable;                 //ID3v2が無い場合はFALSE
    CharEncoding m_encoding;        // 文字エンコードタイプ (0=ISO-8859-1/1=UTF-16/2=UTF-16BE/3=UTF-8)
    FILETYPE m_filetype;            //
    BOOL m_bUnSynchronization;      //非同期化する
    ID3HEAD m_head;
    WORD m_wVer;
    typedef std::multimap<DWORD,CId3Frame> FrameMap;
    FrameMap m_frames;
    CString m_strDefaultEnc;           //TENCのデフォルト値
//    WORD m_wDefaultId3TagVersion;    // ID3V2の初期値(新規作成時にこのバージョンとなる)
public:
    //by Kobarin
    //SetEncode の結果を反映させる
    void ApplyStringEncode(void);
};

#endif // !defined(AFX_ID3TAGV2_H__92584511_76E1_4EE6_90A7_E196C9C0EF88__INCLUDED_)

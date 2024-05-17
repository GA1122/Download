static bool LookupAltName(const String& name, String& altName)
{
    struct FontCodepage {
        const WCHAR* name;
        int codePage;
    };

    struct NamePair {
        const WCHAR* name;
        FontCodepage altNameCodepage;
    };

    const int japaneseCodepage = 932;
    const int simplifiedChineseCodepage = 936;
    const int koreanCodepage = 949;
    const int traditionalChineseCodepage = 950;

    static const NamePair namePairs[] = {
        {L"\xFF2D\xFF33 \xFF30\x30B4\x30B7\x30C3\x30AF", {L"MS PGothic", japaneseCodepage}},
        {L"ms pgothic", {L"\xFF2D\xFF33 \xFF30\x30B4\x30B7\x30C3\x30AF", japaneseCodepage}},
        {L"\xFF2D\xFF33 \xFF30\x660E\x671D", {L"MS PMincho", japaneseCodepage}},
        {L"ms pmincho", {L"\xFF2D\xFF33 \xFF30\x660E\x671D", japaneseCodepage}},
        {L"\xFF2D\xFF33 \x30B4\x30B7\x30C3\x30AF", {L"MS Gothic", japaneseCodepage}},
        {L"ms gothic", {L"\xFF2D\xFF33 \x30B4\x30B7\x30C3\x30AF", japaneseCodepage}},
        {L"\xFF2D\xFF33 \x660E\x671D", {L"MS Mincho", japaneseCodepage}},
        {L"ms mincho", {L"\xFF2D\xFF33 \x660E\x671D", japaneseCodepage}},
        {L"\x30E1\x30A4\x30EA\x30AA", {L"Meiryo", japaneseCodepage}},
        {L"meiryo", {L"\x30E1\x30A4\x30EA\x30AA", japaneseCodepage}},
        {L"\xBC14\xD0D5", {L"Batang", koreanCodepage}},
        {L"batang", {L"\xBC14\xD0D5", koreanCodepage}},
        {L"\xBC14\xD0D5\xCCB4", {L"Batangche", koreanCodepage}},
        {L"batangche", {L"\xBC14\xD0D5\xCCB4", koreanCodepage}},
        {L"\xAD74\xB9BC", {L"Gulim", koreanCodepage}},
        {L"gulim", {L"\xAD74\xB9BC", koreanCodepage}},
        {L"\xAD74\xB9BC\xCCB4", {L"Gulimche", koreanCodepage}},
        {L"gulimche", {L"\xAD74\xB9BC\xCCB4", koreanCodepage}},
        {L"\xB3CB\xC6C0", {L"Dotum", koreanCodepage}},
        {L"dotum", {L"\xB3CB\xC6C0", koreanCodepage}},
        {L"\xB3CB\xC6C0\xCCB4", {L"Dotumche", koreanCodepage}},
        {L"dotumche", {L"\xB3CB\xC6C0\xCCB4", koreanCodepage}},
        {L"\xAD81\xC11C", {L"Gungsuh", koreanCodepage}},
        {L"gungsuh", {L"\xAD81\xC11C", koreanCodepage}},
        {L"\xAD81\xC11C\xCCB4", {L"Gungsuhche", koreanCodepage}},
        {L"gungsuhche", {L"\xAD81\xC11C\xCCB4", koreanCodepage}},
        {L"\xB9D1\xC740 \xACE0\xB515", {L"Malgun Gothic", koreanCodepage}},
        {L"malgun gothic", {L"\xB9D1\xC740 \xACE0\xB515", koreanCodepage}},
        {L"\x5B8B\x4F53", {L"SimSun", simplifiedChineseCodepage}},
        {L"simsun", {L"\x5B8B\x4F53", simplifiedChineseCodepage}},
        {L"\x5B8B\x4F53-ExtB", {L"SimSun-ExtB", simplifiedChineseCodepage}},
        {L"simsun-extb", {L"\x5B8B\x4F53-extb", simplifiedChineseCodepage}},
        {L"\x9ED1\x4F53", {L"SimHei", simplifiedChineseCodepage}},
        {L"simhei", {L"\x9ED1\x4F53", simplifiedChineseCodepage}},
        {L"\x65B0\x5B8B\x4F53", {L"NSimSun", simplifiedChineseCodepage}},
        {L"nsimsun", {L"\x65B0\x5B8B\x4F53", simplifiedChineseCodepage}},
        {L"\x5FAE\x8F6F\x96C5\x9ED1", {L"Microsoft Yahei", simplifiedChineseCodepage}},
        {L"microsoft yahei", {L"\x5FAE\x8F6F\x96C5\x9ED1", simplifiedChineseCodepage}},
        {L"\x4EFF\x5B8B",  {L"FangSong", simplifiedChineseCodepage}},
        {L"fangsong", {L"\x4EFF\x5B8B", simplifiedChineseCodepage}},
        {L"\x6977\x4F53", {L"KaiTi", simplifiedChineseCodepage}},
        {L"kaiti", {L"\x6977\x4F53", simplifiedChineseCodepage}},
        {L"\x4EFF\x5B8B_GB2312",  {L"FangSong_GB2312", simplifiedChineseCodepage}},
        {L"fangsong_gb2312", {L"\x4EFF\x5B8B_gb2312", simplifiedChineseCodepage}},
        {L"\x6977\x4F53", {L"KaiTi_GB2312", simplifiedChineseCodepage}},
        {L"kaiti_gb2312", {L"\x6977\x4F53_gb2312", simplifiedChineseCodepage}},
        {L"\x65B0\x7D30\x660E\x9AD4", {L"PMingLiu", traditionalChineseCodepage}},
        {L"pmingliu", {L"\x65B0\x7D30\x660E\x9AD4", traditionalChineseCodepage}},
        {L"\x65B0\x7D30\x660E\x9AD4-ExtB", {L"PMingLiu-ExtB", traditionalChineseCodepage}},
        {L"pmingliu-extb", {L"\x65B0\x7D30\x660E\x9AD4-extb", traditionalChineseCodepage}},
        {L"\x7D30\x660E\x9AD4", {L"MingLiu", traditionalChineseCodepage}},
        {L"mingliu", {L"\x7D30\x660E\x9AD4", traditionalChineseCodepage}},
        {L"\x7D30\x660E\x9AD4-ExtB", {L"MingLiu-ExtB", traditionalChineseCodepage}},
        {L"mingliu-extb", {L"x65B0\x7D30\x660E\x9AD4-extb", traditionalChineseCodepage}},
        {L"\x5FAE\x8EDF\x6B63\x9ED1\x9AD4", {L"Microsoft JhengHei", traditionalChineseCodepage}},
        {L"microsoft jhengHei", {L"\x5FAE\x8EDF\x6B63\x9ED1\x9AD4", traditionalChineseCodepage}},
        {L"\x6A19\x6977\x9AD4", {L"DFKai-SB", traditionalChineseCodepage}},
        {L"dfkai-sb", {L"\x6A19\x6977\x9AD4", traditionalChineseCodepage}},
        {L"\x6587\x6cc9\x9a5b\x6b63\x9ed1", {L"WenQuanYi Zen Hei", traditionalChineseCodepage}},
        {L"wenquanyi zen hei", {L"\x6587\x6cc9\x9a5b\x6b63\x9ed1", traditionalChineseCodepage}},
        {L"\x6587\x6cc9\x9a7f\x6b63\x9ed1", {L"WenQuanYi Zen Hei", simplifiedChineseCodepage}},
        {L"wenquanyi zen hei", {L"\x6587\x6cc9\x9a7f\x6b63\x9ed1", simplifiedChineseCodepage}},
        {L"\x6587\x9f0e\x0050\x004c\x7d30\x4e0a\x6d77\x5b8b\x0055\x006e\x0069",
         {L"AR PL ShanHeiSun Uni", traditionalChineseCodepage}},
        {L"ar pl shanheisun uni",
         {L"\x6587\x9f0e\x0050\x004c\x7d30\x4e0a\x6d77\x5b8b\x0055\x006e\x0069", traditionalChineseCodepage}},
        {L"\x6587\x9f0e\x0050\x004c\x7ec6\x4e0a\x6d77\x5b8b\x0055\x006e\x0069",
         {L"AR PL ShanHeiSun Uni", simplifiedChineseCodepage}},
        {L"ar pl shanheisun uni",
         {L"\x6587\x9f0e\x0050\x004c\x7ec6\x4e0a\x6d77\x5b8b\x0055\x006e\x0069", simplifiedChineseCodepage}},
        {L"\x6587\x0050\x004C\x4E2D\x6977\x0055\x006E\x0069", {L"AR PL ZenKai Uni", traditionalChineseCodepage}},
        {L"ar pl zenkai uni", {L"\x6587\x0050\x004C\x4E2D\x6977\x0055\x006E\x0069", traditionalChineseCodepage}},
        {L"\x6587\x0050\x004C\x4E2D\x6977\x0055\x006E\x0069", {L"AR PL ZenKai Uni", simplifiedChineseCodepage}},
        {L"ar pl zenkai uni", {L"\x6587\x0050\x004C\x4E2D\x6977\x0055\x006E\x0069", simplifiedChineseCodepage}},
    };

    typedef HashMap<String, const FontCodepage*> NameMap;
    static NameMap* fontNameMap = 0;

    if (!fontNameMap) {
        fontNameMap = new NameMap;
        for (size_t i = 0; i < WTF_ARRAY_LENGTH(namePairs); ++i)
            fontNameMap->set(String(namePairs[i].name), &(namePairs[i].altNameCodepage));
    }

    bool isAscii = false;
    String n;
    if (name.containsOnlyASCII()) {
        isAscii = true;
        n = name.lower();
    } else
        n = name;

    NameMap::iterator iter = fontNameMap->find(n);
    if (iter == fontNameMap->end())
        return false;

    static int systemCp = ::GetACP();
    int fontCp = iter->value->codePage;

    if ((isAscii && systemCp == fontCp) || (!isAscii && systemCp != fontCp)) {
        altName = String(iter->value->name);
        return true;
    }

    return false;
}

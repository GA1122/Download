PassRefPtr<SimpleFontData> FontCache::getFontDataForCharacter(const Font& font, UChar32 inputC)
{
    UChar codeUnits[2];
    int codeUnitsLength;
    if (inputC <= 0xFFFF) {
        codeUnits[0] = inputC;
        codeUnitsLength = 1;
    } else {
        codeUnits[0] = U16_LEAD(inputC);
        codeUnits[1] = U16_TRAIL(inputC);
        codeUnitsLength = 2;
    }

    FontDescription fontDescription = font.fontDescription();
    UChar32 c;
    UScriptCode script;
    const wchar_t* family = getFallbackFamily(codeUnits, codeUnitsLength, fontDescription.genericFamily(), &c, &script);
    FontPlatformData* data = 0;
    if (family)
        data = getFontResourcePlatformData(font.fontDescription(),  AtomicString(family, wcslen(family)), false);

    const static wchar_t* const cjkFonts[] = {
        L"arial unicode ms",
        L"ms pgothic",
        L"simsun",
        L"gulim",
        L"pmingliu",
        L"wenquanyi zen hei",  
        L"ar pl shanheisun uni",
        L"ar pl zenkai uni",
        L"han nom a",   
        L"code2000",    
    };

    const static wchar_t* const commonFonts[] = {
        L"tahoma",
        L"arial unicode ms",
        L"lucida sans unicode",
        L"microsoft sans serif",
        L"palatino linotype",
        L"dejavu serif",
        L"dejavu sasns",
        L"freeserif",
        L"freesans",
        L"gentium",
        L"gentiumalt",
        L"ms pgothic",
        L"simsun",
        L"gulim",
        L"pmingliu",
        L"code2000",
    };

    const wchar_t* const* panUniFonts = 0;
    int numFonts = 0;
    if (script == USCRIPT_HAN) {
        panUniFonts = cjkFonts;
        numFonts = WTF_ARRAY_LENGTH(cjkFonts);
    } else {
        panUniFonts = commonFonts;
        numFonts = WTF_ARRAY_LENGTH(commonFonts);
    }
    int i;
    for (i = 0; (!data || !fontContainsCharacter(data, family, c)) && i < numFonts; ++i) {
        family = panUniFonts[i];
        data = getFontResourcePlatformData(font.fontDescription(), AtomicString(family, wcslen(family)));
    }
    if (i <= numFonts)
        return getFontResourceData(data, DoNotRetain);

    return 0;

}

FontPlatformData* FontCache::createFontPlatformData(const FontDescription& fontDescription, const AtomicString& family)
{
    LOGFONT winfont = {0};
    FillLogFont(fontDescription, &winfont);

    String winName;
    HFONT hfont = createFontIndirectAndGetWinName(family, &winfont, &winName);
    if (!hfont)
        return 0;

    if (!equalIgnoringCase(family, winName)) {
        String altName;
        if (!LookupAltName(family, altName) || !equalIgnoringCase(altName, winName)) {
            DeleteObject(hfont);
            return 0;
        }
    }

    return new FontPlatformData(hfont,
                                fontDescription.computedPixelSize(),
                                fontDescription.orientation());
}

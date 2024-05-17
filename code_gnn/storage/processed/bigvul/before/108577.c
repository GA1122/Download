PassRefPtr<SimpleFontData> FontCache::getLastResortFallbackFont(const FontDescription& description, ShouldRetain shouldRetain)
{
    FontDescription::GenericFamilyType generic = description.genericFamily();

    static AtomicString timesStr("Times New Roman");
    static AtomicString courierStr("Courier New");
    static AtomicString arialStr("Arial");

    AtomicString& fontStr = timesStr;
    if (generic == FontDescription::SansSerifFamily)
        fontStr = arialStr;
    else if (generic == FontDescription::MonospaceFamily)
        fontStr = courierStr;

    RefPtr<SimpleFontData> simpleFont = getFontResourceData(description, fontStr, false, shouldRetain);
    if (simpleFont)
        return simpleFont.release();

    static wchar_t fallbackFontName[LF_FACESIZE] = {0};
    if (fallbackFontName[0])
        return getFontResourceData(description, fallbackFontName, false, shouldRetain);

    if (HFONT defaultGUIFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT))) {
        LOGFONT defaultGUILogFont;
        GetObject(defaultGUIFont, sizeof(defaultGUILogFont), &defaultGUILogFont);
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, defaultGUILogFont, fallbackFontName))
            return simpleFont.release();
    }

    NONCLIENTMETRICS nonClientMetrics = {0};
    nonClientMetrics.cbSize = sizeof(nonClientMetrics);
    if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(nonClientMetrics), &nonClientMetrics, 0)) {
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, nonClientMetrics.lfMessageFont, fallbackFontName))
            return simpleFont.release();
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, nonClientMetrics.lfMenuFont, fallbackFontName))
            return simpleFont.release();
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, nonClientMetrics.lfStatusFont, fallbackFontName))
            return simpleFont.release();
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, nonClientMetrics.lfCaptionFont, fallbackFontName))
            return simpleFont.release();
        if (simpleFont = fontDataFromDescriptionAndLogFont(description, shouldRetain, nonClientMetrics.lfSmCaptionFont, fallbackFontName))
            return simpleFont.release();
    }

    HWndDC dc(0);
    if (dc) {
        GetLastResortFallbackFontProcData procData(this, &description, shouldRetain, fallbackFontName);
        EnumFontFamilies(dc, 0, getLastResortFallbackFontProc, reinterpret_cast<LPARAM>(&procData));

        if (procData.m_fontData)
            return procData.m_fontData.release();
    }

    ASSERT_NOT_REACHED();
    return 0;
}

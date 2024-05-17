PassRefPtr<SimpleFontData> FontCache::fontDataFromDescriptionAndLogFont(const FontDescription& fontDescription, ShouldRetain shouldRetain, const LOGFONT& font, wchar_t* outFontFamilyName)
{
    RefPtr<SimpleFontData> fontData = getFontResourceData(fontDescription, font.lfFaceName, false, shouldRetain);
    if (fontData)
        memcpy(outFontFamilyName, font.lfFaceName, sizeof(font.lfFaceName));
    return fontData.release();
}

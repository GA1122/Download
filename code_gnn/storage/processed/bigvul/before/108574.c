static bool fontContainsCharacter(const FontPlatformData* fontData,
                                  const wchar_t* family, UChar32 character)
{
    if (character > 0xFFFF)
        return true;

    static FontCmapCache* fontCmapCache = 0;
    if (!fontCmapCache)
        fontCmapCache = new FontCmapCache;

    HashMap<const wchar_t*, icu::UnicodeSet*>::iterator it = fontCmapCache->find(family);
    if (it != fontCmapCache->end())
        return it->value->contains(character);

    HFONT hfont = fontData->hfont();
    HWndDC hdc(0);
    HGDIOBJ oldFont = static_cast<HFONT>(SelectObject(hdc, hfont));
    int count = GetFontUnicodeRanges(hdc, 0);
    if (!count && FontPlatformData::ensureFontLoaded(hfont))
        count = GetFontUnicodeRanges(hdc, 0);
    if (!count) {
        LOG_ERROR("Unable to get the font unicode range after second attempt");
        SelectObject(hdc, oldFont);
        return true;
    }

    static Vector<char, 512> glyphsetBuffer;
    glyphsetBuffer.resize(GetFontUnicodeRanges(hdc, 0));
    GLYPHSET* glyphset = reinterpret_cast<GLYPHSET*>(glyphsetBuffer.data());
    count = GetFontUnicodeRanges(hdc, glyphset);
    ASSERT(count > 0);
    SelectObject(hdc, oldFont);

    unsigned i = 0;
    icu::UnicodeSet* cmap = new icu::UnicodeSet;
    while (i < glyphset->cRanges) {
        WCHAR start = glyphset->ranges[i].wcLow;
        cmap->add(start, start + glyphset->ranges[i].cGlyphs - 1);
        i++;
    }
    cmap->freeze();
    fontCmapCache->set(family, cmap);
    return cmap->contains(character);
}

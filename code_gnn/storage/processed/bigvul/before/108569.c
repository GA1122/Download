    GetLastResortFallbackFontProcData(FontCache* fontCache, const FontDescription* fontDescription, FontCache::ShouldRetain shouldRetain, wchar_t* fontName)
        : m_fontCache(fontCache)
        , m_fontDescription(fontDescription)
        , m_shouldRetain(shouldRetain)
        , m_fontName(fontName)
        , m_fontData(0)
    {
    }

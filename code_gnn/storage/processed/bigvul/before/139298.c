void HarfBuzzShaper::addHarfBuzzRun(unsigned startCharacter,
    unsigned endCharacter, const SimpleFontData* fontData,
    UScriptCode script)
{
    ASSERT(endCharacter > startCharacter);
    ASSERT(script != USCRIPT_INVALID_CODE);
    if (m_fallbackFonts)
        trackNonPrimaryFallbackFont(fontData);
    return m_harfBuzzRuns.append(HarfBuzzRun::create(fontData,
        startCharacter, endCharacter - startCharacter,
        TextDirectionToHBDirection(m_run.direction(), m_font->fontDescription().orientation(), fontData),
        ICUScriptToHBScript(script)));
}

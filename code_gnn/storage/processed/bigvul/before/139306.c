bool HarfBuzzShaper::createHarfBuzzRunsForSingleCharacter()
{
    ASSERT(m_normalizedBufferLength == 1);
    UChar32 character = m_normalizedBuffer[0];
    if (!U16_IS_SINGLE(character))
        return false;
    const SimpleFontData* fontData = m_font->glyphDataForCharacter(character, false, m_run.normalizeSpace()).fontData;
    UErrorCode errorCode = U_ZERO_ERROR;
    UScriptCode script = uscript_getScript(character, &errorCode);
    if (U_FAILURE(errorCode))
        return false;
    addHarfBuzzRun(0, 1, fontData, script);
    return true;
}

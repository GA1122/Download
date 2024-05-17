inline void HarfBuzzShaper::HarfBuzzRun::applyShapeResult(hb_buffer_t* harfBuzzBuffer)
{
    m_numGlyphs = hb_buffer_get_length(harfBuzzBuffer);
    m_glyphs.resize(m_numGlyphs);
    m_advances.resize(m_numGlyphs);
    m_glyphToCharacterIndexes.resize(m_numGlyphs);
    m_offsets.resize(m_numGlyphs);
}

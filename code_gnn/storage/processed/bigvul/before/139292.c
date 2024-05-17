inline HarfBuzzShaper::HarfBuzzRun::HarfBuzzRun(const HarfBuzzRun& rhs)
    : m_fontData(rhs.m_fontData)
    , m_startIndex(rhs.m_startIndex)
    , m_numCharacters(rhs.m_numCharacters)
    , m_numGlyphs(rhs.m_numGlyphs)
    , m_direction(rhs.m_direction)
    , m_script(rhs.m_script)
    , m_glyphs(rhs.m_glyphs)
    , m_advances(rhs.m_advances)
    , m_glyphToCharacterIndexes(rhs.m_glyphToCharacterIndexes)
    , m_offsets(rhs.m_offsets)
    , m_width(rhs.m_width)
{
}

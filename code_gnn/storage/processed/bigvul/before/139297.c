void HarfBuzzShaper::HarfBuzzRun::addAdvance(unsigned index, float advance)
{
    ASSERT(index < m_numGlyphs);
    m_advances[index] += advance;
}

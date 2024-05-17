float HarfBuzzShaper::fillGlyphBufferFromHarfBuzzRun(GlyphBuffer* glyphBuffer,
    HarfBuzzRun* currentRun, float initialAdvance)
{
    FloatSize* offsets = currentRun->offsets();
    uint16_t* glyphs = currentRun->glyphs();
    float* advances = currentRun->advances();
    unsigned numGlyphs = currentRun->numGlyphs();
    uint16_t* glyphToCharacterIndexes = currentRun->glyphToCharacterIndexes();
    float advanceSoFar = initialAdvance;
    if (m_run.rtl()) {
        for (unsigned i = 0; i < numGlyphs; ++i) {
            uint16_t currentCharacterIndex = currentRun->startIndex() + glyphToCharacterIndexes[i];
            if (currentCharacterIndex >= m_toIndex) {
                advanceSoFar += advances[i];
            } else if (currentCharacterIndex >= m_fromIndex) {
                FloatPoint runStartOffset = HB_DIRECTION_IS_HORIZONTAL(currentRun->direction()) ?
                    FloatPoint(advanceSoFar, 0) : FloatPoint(0, advanceSoFar);
                glyphBuffer->add(glyphs[i], currentRun->fontData(), runStartOffset + offsets[i]);
                advanceSoFar += advances[i];
            }
        }
    } else {
        for (unsigned i = 0; i < numGlyphs; ++i) {
            uint16_t currentCharacterIndex = currentRun->startIndex() + glyphToCharacterIndexes[i];
            if (currentCharacterIndex < m_fromIndex) {
                advanceSoFar += advances[i];
            } else if (currentCharacterIndex < m_toIndex) {
                FloatPoint runStartOffset = HB_DIRECTION_IS_HORIZONTAL(currentRun->direction()) ?
                    FloatPoint(advanceSoFar, 0) : FloatPoint(0, advanceSoFar);
                glyphBuffer->add(glyphs[i], currentRun->fontData(), runStartOffset + offsets[i]);
                advanceSoFar += advances[i];
            }
        }
    }

    return advanceSoFar - initialAdvance;
}

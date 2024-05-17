float HarfBuzzShaper::fillGlyphBufferForTextEmphasis(GlyphBuffer* glyphBuffer, HarfBuzzRun* currentRun, float initialAdvance)
{
    float* advances = currentRun->advances();
    unsigned numGlyphs = currentRun->numGlyphs();
    uint16_t* glyphToCharacterIndexes = currentRun->glyphToCharacterIndexes();
    unsigned graphemesInCluster = 1;
    float clusterAdvance = 0;
    uint16_t clusterStart;


    if (m_run.rtl())
        clusterStart = currentRun->startIndex() + currentRun->numCharacters();
    else
        clusterStart = currentRun->startIndex() + glyphToCharacterIndexes[0];

    float advanceSoFar = initialAdvance;
    for (unsigned i = 0; i < numGlyphs; ++i) {
        uint16_t currentCharacterIndex = currentRun->startIndex() + glyphToCharacterIndexes[i];
        bool isRunEnd = (i + 1 == numGlyphs);
        bool isClusterEnd =  isRunEnd || (currentRun->startIndex() + glyphToCharacterIndexes[i + 1] != currentCharacterIndex);

        if ((m_run.rtl() && currentCharacterIndex >= m_toIndex) || (!m_run.rtl() && currentCharacterIndex < m_fromIndex)) {
            advanceSoFar += advances[i];
            m_run.rtl() ? --clusterStart : ++clusterStart;
            continue;
        }

        clusterAdvance += advances[i];

        if (isClusterEnd) {
            uint16_t clusterEnd;
            if (m_run.rtl())
                clusterEnd = currentCharacterIndex;
            else
                clusterEnd = isRunEnd ? currentRun->startIndex() + currentRun->numCharacters() : currentRun->startIndex() + glyphToCharacterIndexes[i + 1];

            graphemesInCluster = countGraphemesInCluster(m_normalizedBuffer.get(), m_normalizedBufferLength, clusterStart, clusterEnd);
            if (!graphemesInCluster || !clusterAdvance)
                continue;

            float glyphAdvanceX = clusterAdvance / graphemesInCluster;
            for (unsigned j = 0; j < graphemesInCluster; ++j) {
                if (Character::canReceiveTextEmphasis(m_run[currentCharacterIndex]))
                    addEmphasisMark(glyphBuffer, advanceSoFar + glyphAdvanceX / 2);

                advanceSoFar += glyphAdvanceX;
            }
            clusterStart = clusterEnd;
            clusterAdvance = 0;
        }
    }

    return advanceSoFar - initialAdvance;
}

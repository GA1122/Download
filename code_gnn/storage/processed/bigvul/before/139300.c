float HarfBuzzShaper::adjustSpacing(HarfBuzzRun* currentRun, size_t glyphIndex, unsigned currentCharacterIndex, HarfBuzzRun* previousRun, float& offsetX, float& totalAdvance)
{
    float spacing = 0;
    UChar32 character = m_normalizedBuffer[currentCharacterIndex];
    if (m_letterSpacing && !Character::treatAsZeroWidthSpace(character))
        spacing += m_letterSpacing;

    bool treatAsSpace = Character::treatAsSpace(character);
    if (treatAsSpace && currentCharacterIndex && (character != '\t' || !m_run.allowTabs()))
        spacing += m_wordSpacingAdjustment;

    if (!m_expansionOpportunityCount)
        return spacing;

    if (treatAsSpace) {
        spacing += nextExpansionPerOpportunity();
        m_isAfterExpansion = true;
        return spacing;
    }

    if (m_run.textJustify() != TextJustify::TextJustifyAuto) {
        m_isAfterExpansion = false;
        return spacing;
    }

    if (U16_IS_LEAD(character) && currentCharacterIndex + 1 < m_normalizedBufferLength && U16_IS_TRAIL(m_normalizedBuffer[currentCharacterIndex + 1]))
        character = U16_GET_SUPPLEMENTARY(character, m_normalizedBuffer[currentCharacterIndex + 1]);
    if (!Character::isCJKIdeographOrSymbol(character)) {
        m_isAfterExpansion = false;
        return spacing;
    }

    if (!m_isAfterExpansion) {
        float expandBefore = nextExpansionPerOpportunity();
        if (expandBefore) {
            if (glyphIndex > 0) {
                currentRun->addAdvance(glyphIndex - 1, expandBefore);
                totalAdvance += expandBefore;
            } else if (previousRun) {
                previousRun->addAdvance(previousRun->numGlyphs() - 1, expandBefore);
                previousRun->setWidth(previousRun->width() + expandBefore);
                m_totalWidth += expandBefore;
            } else {
                offsetX += expandBefore;
                totalAdvance += expandBefore;
            }
        }
        if (!m_expansionOpportunityCount)
            return spacing;
    }

    spacing += nextExpansionPerOpportunity();
    m_isAfterExpansion = true;
    return spacing;
}

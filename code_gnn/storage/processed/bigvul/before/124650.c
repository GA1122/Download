static RenderStyle* styleForFirstLetter(RenderObject* firstLetterBlock, RenderObject* firstLetterContainer)
{
    RenderStyle* pseudoStyle = firstLetterBlock->getCachedPseudoStyle(FIRST_LETTER, firstLetterContainer->firstLineStyle());
    pseudoStyle->setDisplay(pseudoStyle->isFloating() ? BLOCK : INLINE);
    pseudoStyle->setPosition(StaticPosition);
    return pseudoStyle;
}

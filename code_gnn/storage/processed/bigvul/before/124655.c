void RenderBlock::updateFirstLetter()
{
    if (!document().styleEngine()->usesFirstLetterRules())
        return;
    if (style()->styleType() == FIRST_LETTER)
        return;

    RenderObject* firstLetterBlock = findFirstLetterBlock(this);
    if (!firstLetterBlock)
        return;

    RenderObject* currChild = firstLetterBlock->firstChild();
    unsigned length = 0;
    while (currChild) {
        if (currChild->isText()) {
            length = firstLetterLength(toRenderText(currChild)->originalText());
            if (length)
                break;
            currChild = currChild->nextSibling();
        } else if (currChild->isListMarker()) {
            currChild = currChild->nextSibling();
        } else if (currChild->isFloatingOrOutOfFlowPositioned()) {
            if (currChild->style()->styleType() == FIRST_LETTER) {
                currChild = currChild->firstChild();
                break;
            }
            currChild = currChild->nextSibling();
        } else if (currChild->isReplaced() || currChild->isRenderButton() || currChild->isMenuList())
            break;
        else if (currChild->style()->hasPseudoStyle(FIRST_LETTER) && currChild->canHaveGeneratedChildren())  {
            firstLetterBlock = currChild;
            currChild = currChild->firstChild();
        } else
            currChild = currChild->firstChild();
    }

    if (!currChild)
        return;

    if (currChild->parent()->style()->styleType() == FIRST_LETTER) {
        updateFirstLetterStyle(firstLetterBlock, currChild);
        return;
    }

    if (!currChild->isText() || currChild->isBR() || toRenderText(currChild)->isWordBreak())
        return;

    LayoutStateDisabler layoutStateDisabler(*this);

    createFirstLetterRenderer(firstLetterBlock, currChild, length);
}

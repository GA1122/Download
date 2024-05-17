void RenderBlock::updateFirstLetterStyle(RenderObject* firstLetterBlock, RenderObject* currentChild)
{
    RenderObject* firstLetter = currentChild->parent();
    RenderObject* firstLetterContainer = firstLetter->parent();
    RenderStyle* pseudoStyle = styleForFirstLetter(firstLetterBlock, firstLetterContainer);
    ASSERT(firstLetter->isFloating() || firstLetter->isInline());

    if (RenderStyle::stylePropagationDiff(firstLetter->style(), pseudoStyle) == Reattach) {
        RenderBoxModelObject* newFirstLetter;
        if (pseudoStyle->display() == INLINE)
            newFirstLetter = RenderInline::createAnonymous(&document());
        else
            newFirstLetter = RenderBlockFlow::createAnonymous(&document());
        newFirstLetter->setStyle(pseudoStyle);

        LayoutStateDisabler layoutStateDisabler(*this);
        while (RenderObject* child = firstLetter->firstChild()) {
            if (child->isText())
                toRenderText(child)->removeAndDestroyTextBoxes();
            firstLetter->removeChild(child);
            newFirstLetter->addChild(child, 0);
        }

        RenderObject* nextSibling = firstLetter->nextSibling();
        if (RenderTextFragment* remainingText = toRenderBoxModelObject(firstLetter)->firstLetterRemainingText()) {
            ASSERT(remainingText->isAnonymous() || remainingText->node()->renderer() == remainingText);
            remainingText->setFirstLetter(newFirstLetter);
            newFirstLetter->setFirstLetterRemainingText(remainingText);
        }
        firstLetterContainer->virtualChildren()->removeChildNode(firstLetterContainer, firstLetter);
        firstLetter->destroy();
        firstLetter = newFirstLetter;
        firstLetterContainer->addChild(firstLetter, nextSibling);
    } else
        firstLetter->setStyle(pseudoStyle);

    for (RenderObject* genChild = firstLetter->firstChild(); genChild; genChild = genChild->nextSibling()) {
        if (genChild->isText())
            genChild->setStyle(pseudoStyle);
    }
}

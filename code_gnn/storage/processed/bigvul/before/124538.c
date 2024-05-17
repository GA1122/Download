void RenderBlock::createFirstLetterRenderer(RenderObject* firstLetterBlock, RenderObject* currentChild, unsigned length)
{
    ASSERT(length && currentChild->isText());

    RenderObject* firstLetterContainer = currentChild->parent();
    RenderStyle* pseudoStyle = styleForFirstLetter(firstLetterBlock, firstLetterContainer);
    RenderObject* firstLetter = 0;
    if (pseudoStyle->display() == INLINE)
        firstLetter = RenderInline::createAnonymous(&document());
    else
        firstLetter = RenderBlockFlow::createAnonymous(&document());
    firstLetter->setStyle(pseudoStyle);
    firstLetterContainer->addChild(firstLetter, currentChild);

    RenderText* textObj = toRenderText(currentChild);

    String oldText = textObj->originalText();
    ASSERT(oldText.impl());

    RenderTextFragment* remainingText =
        new RenderTextFragment(textObj->node() ? textObj->node() : &textObj->document(), oldText.impl(), length, oldText.length() - length);
    remainingText->setStyle(textObj->style());
    if (remainingText->node())
        remainingText->node()->setRenderer(remainingText);

    firstLetterContainer->addChild(remainingText, textObj);
    firstLetterContainer->removeChild(textObj);
    remainingText->setFirstLetter(firstLetter);
    toRenderBoxModelObject(firstLetter)->setFirstLetterRemainingText(remainingText);

    RenderTextFragment* letter =
        new RenderTextFragment(remainingText->node() ? remainingText->node() : &remainingText->document(), oldText.impl(), 0, length);
    letter->setStyle(pseudoStyle);
    firstLetter->addChild(letter);

    textObj->destroy();
}

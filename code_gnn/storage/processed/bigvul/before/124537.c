RenderBlock* RenderBlock::createAnonymousWithParentRendererAndDisplay(const RenderObject* parent, EDisplay display)
{
    EDisplay newDisplay;
    RenderBlock* newBox = 0;
    if (display == BOX || display == INLINE_BOX) {
        newBox = RenderDeprecatedFlexibleBox::createAnonymous(&parent->document());
        newDisplay = BOX;
    } else if (display == FLEX || display == INLINE_FLEX) {
        newBox = RenderFlexibleBox::createAnonymous(&parent->document());
        newDisplay = FLEX;
    } else {
        newBox = RenderBlockFlow::createAnonymous(&parent->document());
        newDisplay = BLOCK;
    }

    RefPtr<RenderStyle> newStyle = RenderStyle::createAnonymousStyleWithDisplay(parent->style(), newDisplay);
    newBox->setStyle(newStyle.release());
    return newBox;
}

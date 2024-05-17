RenderBlockFlow* RenderBlock::createAnonymousColumnSpanWithParentRenderer(const RenderObject* parent)
{
    RefPtr<RenderStyle> newStyle = RenderStyle::createAnonymousStyleWithDisplay(parent->style(), BLOCK);
    newStyle->setColumnSpan(ColumnSpanAll);

    RenderBlockFlow* newBox = RenderBlockFlow::createAnonymous(&parent->document());
    newBox->setStyle(newStyle.release());
    return newBox;
}

RenderBlockFlow* RenderBlock::createAnonymousColumnsWithParentRenderer(const RenderObject* parent)
{
    RefPtr<RenderStyle> newStyle = RenderStyle::createAnonymousStyleWithDisplay(parent->style(), BLOCK);
    newStyle->inheritColumnPropertiesFrom(parent->style());

    RenderBlockFlow* newBox = RenderBlockFlow::createAnonymous(&parent->document());
    newBox->setStyle(newStyle.release());
    return newBox;
}

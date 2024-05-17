void RenderBlockFlow::setStaticInlinePositionForChild(RenderBox* child, LayoutUnit blockOffset, LayoutUnit inlinePosition)
{
    child->layer()->setStaticInlinePosition(inlinePosition);
}

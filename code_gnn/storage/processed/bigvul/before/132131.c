void LayoutBlockFlow::setStaticInlinePositionForChild(LayoutBox& child, LayoutUnit inlinePosition)
{
    child.layer()->setStaticInlinePosition(inlinePosition);
}

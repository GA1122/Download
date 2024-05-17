void RenderBlockFlow::updateStaticInlinePositionForChild(RenderBox* child, LayoutUnit logicalTop)
{
    if (child->style()->isOriginalDisplayInlineType())
        setStaticInlinePositionForChild(child, logicalTop, startAlignedOffsetForLine(logicalTop, false));
    else
        setStaticInlinePositionForChild(child, logicalTop, startOffsetForContent());
}

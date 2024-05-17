void LayoutBlockFlow::updateStaticInlinePositionForChild(LayoutBox& child, LayoutUnit logicalTop)
{
    if (child.style()->isOriginalDisplayInlineType())
        setStaticInlinePositionForChild(child, startAlignedOffsetForLine(logicalTop, false));
    else
        setStaticInlinePositionForChild(child, startOffsetForContent());
}

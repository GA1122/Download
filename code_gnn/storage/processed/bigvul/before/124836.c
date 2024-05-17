LayoutUnit RenderBox::computeLogicalHeightUsing(const Length& height, LayoutUnit intrinsicContentHeight) const
{
    LayoutUnit logicalHeight = computeContentAndScrollbarLogicalHeightUsing(height, intrinsicContentHeight);
    if (logicalHeight != -1)
        logicalHeight = adjustBorderBoxLogicalHeightForBoxSizing(logicalHeight);
    return logicalHeight;
}

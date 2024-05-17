LayoutUnit RenderBox::constrainContentBoxLogicalHeightByMinMax(LayoutUnit logicalHeight, LayoutUnit intrinsicContentHeight) const
{
    RenderStyle* styleToUse = style();
    if (!styleToUse->logicalMaxHeight().isUndefined()) {
        LayoutUnit maxH = computeContentLogicalHeight(styleToUse->logicalMaxHeight(), intrinsicContentHeight);
        if (maxH != -1)
            logicalHeight = min(logicalHeight, maxH);
    }
    return max(logicalHeight, computeContentLogicalHeight(styleToUse->logicalMinHeight(), intrinsicContentHeight));
}

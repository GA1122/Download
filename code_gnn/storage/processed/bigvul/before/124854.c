LayoutUnit RenderBox::constrainLogicalWidthByMinMax(LayoutUnit logicalWidth, LayoutUnit availableWidth, RenderBlock* cb) const
{
    RenderStyle* styleToUse = style();
    if (!styleToUse->logicalMaxWidth().isUndefined())
        logicalWidth = min(logicalWidth, computeLogicalWidthUsing(MaxSize, styleToUse->logicalMaxWidth(), availableWidth, cb));
    return max(logicalWidth, computeLogicalWidthUsing(MinSize, styleToUse->logicalMinWidth(), availableWidth, cb));
}

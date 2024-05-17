LayoutUnit RenderBox::containingBlockAvailableLineWidth() const
{
    RenderBlock* cb = containingBlock();
    if (cb->isRenderBlockFlow())
        return toRenderBlockFlow(cb)->availableLogicalWidthForLine(logicalTop(), false, availableLogicalHeight(IncludeMarginBorderPadding));
    return 0;
}

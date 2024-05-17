void RenderBlock::computeIntrinsicLogicalWidths(LayoutUnit& minLogicalWidth, LayoutUnit& maxLogicalWidth) const
{
    if (childrenInline()) {
        toRenderBlockFlow(const_cast<RenderBlock*>(this))->computeInlinePreferredLogicalWidths(minLogicalWidth, maxLogicalWidth);
    } else {
        computeBlockPreferredLogicalWidths(minLogicalWidth, maxLogicalWidth);
    }

    maxLogicalWidth = max(minLogicalWidth, maxLogicalWidth);

    adjustIntrinsicLogicalWidthsForColumns(minLogicalWidth, maxLogicalWidth);

    if (childrenInline() && isMarquee() && toRenderMarquee(this)->isHorizontal())
        minLogicalWidth = 0;

    if (isTableCell()) {
        Length tableCellWidth = toRenderTableCell(this)->styleOrColLogicalWidth();
        if (tableCellWidth.isFixed() && tableCellWidth.value() > 0)
            maxLogicalWidth = max(minLogicalWidth, adjustContentBoxLogicalWidthForBoxSizing(tableCellWidth.value()));
    }

    int scrollbarWidth = instrinsicScrollbarLogicalWidth();
    maxLogicalWidth += scrollbarWidth;
    minLogicalWidth += scrollbarWidth;
}

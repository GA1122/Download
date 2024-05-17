void RenderBlock::adjustIntrinsicLogicalWidthsForColumns(LayoutUnit& minLogicalWidth, LayoutUnit& maxLogicalWidth) const
{
    if (!style()->hasAutoColumnCount() || !style()->hasAutoColumnWidth()) {
        int columnCount = style()->hasAutoColumnCount() ? 1 : style()->columnCount();
        LayoutUnit columnWidth;
        LayoutUnit gapExtra = (columnCount - 1) * columnGap();
        if (style()->hasAutoColumnWidth()) {
            minLogicalWidth = minLogicalWidth * columnCount + gapExtra;
        } else {
            columnWidth = style()->columnWidth();
            minLogicalWidth = min(minLogicalWidth, columnWidth);
        }
        maxLogicalWidth = max(maxLogicalWidth, columnWidth) * columnCount + gapExtra;
    }
}

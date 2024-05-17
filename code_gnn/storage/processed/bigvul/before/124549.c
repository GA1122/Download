LayoutPoint RenderBlock::flipForWritingModeIncludingColumns(const LayoutPoint& point) const
{
    ASSERT(hasColumns());
    if (!hasColumns() || !style()->isFlippedBlocksWritingMode())
        return point;
    ColumnInfo* colInfo = columnInfo();
    LayoutUnit columnLogicalHeight = colInfo->columnHeight();
    LayoutUnit expandedLogicalHeight = borderBefore() + paddingBefore() + columnCount(colInfo) * columnLogicalHeight + borderAfter() + paddingAfter() + scrollbarLogicalHeight();
    if (isHorizontalWritingMode())
        return LayoutPoint(point.x(), expandedLogicalHeight - point.y());
    return LayoutPoint(expandedLogicalHeight - point.x(), point.y());
}

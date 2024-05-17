void RenderBlock::adjustRectForColumns(LayoutRect& r) const
{
    if (!hasColumns())
        return;

    ColumnInfo* colInfo = columnInfo();

    unsigned colCount = columnCount(colInfo);
    if (!colCount)
        return;

    LayoutRect result;

    bool isHorizontal = isHorizontalWritingMode();
    LayoutUnit beforeBorderPadding = borderBefore() + paddingBefore();
    LayoutUnit colHeight = colInfo->columnHeight();
    if (!colHeight)
        return;

    LayoutUnit startOffset = max(isHorizontal ? r.y() : r.x(), beforeBorderPadding);
    LayoutUnit endOffset = max(min<LayoutUnit>(isHorizontal ? r.maxY() : r.maxX(), beforeBorderPadding + colCount * colHeight), beforeBorderPadding);

    unsigned startColumn = (startOffset - beforeBorderPadding) / colHeight;
    unsigned endColumn = (endOffset - beforeBorderPadding) / colHeight;

    if (startColumn == endColumn) {
        LayoutUnit logicalLeftOffset = logicalLeftOffsetForContent();
        LayoutRect colRect = columnRectAt(colInfo, startColumn);
        LayoutRect repaintRect = r;

        if (colInfo->progressionAxis() == ColumnInfo::InlineAxis) {
            if (isHorizontal)
                repaintRect.move(colRect.x() - logicalLeftOffset, - static_cast<int>(startColumn) * colHeight);
            else
                repaintRect.move(- static_cast<int>(startColumn) * colHeight, colRect.y() - logicalLeftOffset);
        } else {
            if (isHorizontal)
                repaintRect.move(0, colRect.y() - startColumn * colHeight - beforeBorderPadding);
            else
                repaintRect.move(colRect.x() - startColumn * colHeight - beforeBorderPadding, 0);
        }
        repaintRect.intersect(colRect);
        result.unite(repaintRect);
    } else {
        result.unite(columnRectAt(colInfo, startColumn));
        result.unite(columnRectAt(colInfo, endColumn));
    }

    r = result;
}

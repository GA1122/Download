LayoutRect RenderBlock::columnRectAt(ColumnInfo* colInfo, unsigned index) const
{
    ASSERT(hasColumns() && gColumnInfoMap->get(this) == colInfo);

    LayoutUnit colLogicalWidth = colInfo->desiredColumnWidth();
    LayoutUnit colLogicalHeight = colInfo->columnHeight();
    LayoutUnit colLogicalTop = borderBefore() + paddingBefore();
    LayoutUnit colLogicalLeft = logicalLeftOffsetForContent();
    LayoutUnit colGap = columnGap();
    if (colInfo->progressionAxis() == ColumnInfo::InlineAxis) {
        if (style()->isLeftToRightDirection())
            colLogicalLeft += index * (colLogicalWidth + colGap);
        else
            colLogicalLeft += contentLogicalWidth() - colLogicalWidth - index * (colLogicalWidth + colGap);
    } else {
        colLogicalTop += index * (colLogicalHeight + colGap);
    }

    if (isHorizontalWritingMode())
        return LayoutRect(colLogicalLeft, colLogicalTop, colLogicalWidth, colLogicalHeight);
    return LayoutRect(colLogicalTop, colLogicalLeft, colLogicalHeight, colLogicalWidth);
}

void RenderBlockFlow::setColumnCountAndHeight(unsigned count, LayoutUnit pageLogicalHeight)
{
    ColumnInfo* colInfo = columnInfo();
    if (pageLogicalHeight)
        colInfo->setColumnCountAndHeight(count, pageLogicalHeight);

    if (columnCount(colInfo)) {
        setLogicalHeight(borderBefore() + paddingBefore() + colInfo->columnHeight() + borderAfter() + paddingAfter() + scrollbarLogicalHeight());
        m_overflow.clear();
    }
}

bool RenderBlockFlow::shouldRelayoutForPagination(LayoutUnit& pageLogicalHeight, LayoutUnit layoutOverflowLogicalBottom) const
{
    ColumnInfo* colInfo = columnInfo();
    LayoutUnit columnHeight = pageLogicalHeight;
    const int minColumnCount = colInfo->forcedBreaks() + 1;
    const int desiredColumnCount = colInfo->desiredColumnCount();
    if (minColumnCount >= desiredColumnCount) {
        if (!pageLogicalHeight) {
            LayoutUnit distanceBetweenBreaks = max<LayoutUnit>(colInfo->maximumDistanceBetweenForcedBreaks(),
                view()->layoutState()->pageLogicalOffset(*this, borderBefore() + paddingBefore() + layoutOverflowLogicalBottom) - colInfo->forcedBreakOffset());
            columnHeight = max(colInfo->minimumColumnHeight(), distanceBetweenBreaks);
        }
    } else if (layoutOverflowLogicalBottom > boundedMultiply(pageLogicalHeight, desiredColumnCount)) {
        columnHeight = max<LayoutUnit>(colInfo->minimumColumnHeight(), ceilf(layoutOverflowLogicalBottom.toFloat() / desiredColumnCount));
    }

    if (columnHeight && columnHeight != pageLogicalHeight) {
        pageLogicalHeight = columnHeight;
        return true;
    }

    return false;
}

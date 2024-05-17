void RenderBlockFlow::checkForPaginationLogicalHeightChange(LayoutUnit& pageLogicalHeight, bool& pageLogicalHeightChanged, bool& hasSpecifiedPageLogicalHeight)
{
    if (RenderMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        LogicalExtentComputedValues computedValues;
        computeLogicalHeight(LayoutUnit(), logicalTop(), computedValues);
        LayoutUnit columnHeight = computedValues.m_extent - borderAndPaddingLogicalHeight() - scrollbarLogicalHeight();
        pageLogicalHeightChanged = columnHeight != flowThread->columnHeightAvailable();
        flowThread->setColumnHeightAvailable(std::max<LayoutUnit>(columnHeight, 0));
    } else if (hasColumns()) {
        ColumnInfo* colInfo = columnInfo();

        if (!pageLogicalHeight) {
            LayoutUnit oldLogicalHeight = logicalHeight();
            setLogicalHeight(0);
            updateLogicalHeight();
            LayoutUnit columnHeight = contentLogicalHeight();
            if (columnHeight > 0) {
                pageLogicalHeight = columnHeight;
                hasSpecifiedPageLogicalHeight = true;
            }
            setLogicalHeight(oldLogicalHeight);
        }
        if (colInfo->columnHeight() != pageLogicalHeight && everHadLayout()) {
            colInfo->setColumnHeight(pageLogicalHeight);
            pageLogicalHeightChanged = true;
        }

        if (!hasSpecifiedPageLogicalHeight && !pageLogicalHeight)
            colInfo->clearForcedBreaks();
    } else if (isRenderFlowThread()) {
        RenderFlowThread* flowThread = toRenderFlowThread(this);

        pageLogicalHeight = flowThread->isPageLogicalHeightKnown() ? LayoutUnit(1) : LayoutUnit(0);

        pageLogicalHeightChanged = flowThread->pageLogicalSizeChanged();
    }
}

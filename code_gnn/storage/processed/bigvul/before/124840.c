LayoutUnit RenderBox::computePercentageLogicalHeight(const Length& height) const
{
    LayoutUnit availableHeight = -1;

    bool skippedAutoHeightContainingBlock = false;
    RenderBlock* cb = containingBlock();
    const RenderBox* containingBlockChild = this;
    LayoutUnit rootMarginBorderPaddingHeight = 0;
    while (!cb->isRenderView() && skipContainingBlockForPercentHeightCalculation(cb)) {
        if (cb->isBody() || cb->isDocumentElement())
            rootMarginBorderPaddingHeight += cb->marginBefore() + cb->marginAfter() + cb->borderAndPaddingLogicalHeight();
        skippedAutoHeightContainingBlock = true;
        containingBlockChild = cb;
        cb = cb->containingBlock();
    }
    cb->addPercentHeightDescendant(const_cast<RenderBox*>(this));

    RenderStyle* cbstyle = cb->style();

    bool isOutOfFlowPositionedWithSpecifiedHeight = cb->isOutOfFlowPositioned() && (!cbstyle->logicalHeight().isAuto() || (!cbstyle->logicalTop().isAuto() && !cbstyle->logicalBottom().isAuto()));

    bool includeBorderPadding = isTable();

    if (isHorizontalWritingMode() != cb->isHorizontalWritingMode())
        availableHeight = containingBlockChild->containingBlockLogicalWidthForContent();
    else if (hasOverrideContainingBlockLogicalHeight())
        availableHeight = overrideContainingBlockContentLogicalHeight();
    else if (cb->isTableCell()) {
        if (!skippedAutoHeightContainingBlock) {
            if (!cb->hasOverrideHeight()) {
                RenderTableCell* cell = toRenderTableCell(cb);
                if (scrollsOverflowY() && (!cell->style()->logicalHeight().isAuto() || !cell->table()->style()->logicalHeight().isAuto()))
                    return 0;
                return -1;
            }
            availableHeight = cb->overrideLogicalContentHeight();
            includeBorderPadding = true;
        }
    } else if (cbstyle->logicalHeight().isFixed()) {
        LayoutUnit contentBoxHeight = cb->adjustContentBoxLogicalHeightForBoxSizing(cbstyle->logicalHeight().value());
        availableHeight = max<LayoutUnit>(0, cb->constrainContentBoxLogicalHeightByMinMax(contentBoxHeight - cb->scrollbarLogicalHeight(), -1));
    } else if (cbstyle->logicalHeight().isPercent() && !isOutOfFlowPositionedWithSpecifiedHeight) {
        LayoutUnit heightWithScrollbar = cb->computePercentageLogicalHeight(cbstyle->logicalHeight());
        if (heightWithScrollbar != -1) {
            LayoutUnit contentBoxHeightWithScrollbar = cb->adjustContentBoxLogicalHeightForBoxSizing(heightWithScrollbar);
            LayoutUnit contentBoxHeight = cb->constrainContentBoxLogicalHeightByMinMax(contentBoxHeightWithScrollbar - cb->scrollbarLogicalHeight(), -1);
            availableHeight = max<LayoutUnit>(0, contentBoxHeight);
        }
    } else if (isOutOfFlowPositionedWithSpecifiedHeight) {
        LogicalExtentComputedValues computedValues;
        cb->computeLogicalHeight(cb->logicalHeight(), 0, computedValues);
        availableHeight = computedValues.m_extent - cb->borderAndPaddingLogicalHeight() - cb->scrollbarLogicalHeight();
    } else if (cb->isRenderView())
        availableHeight = viewLogicalHeightForPercentages();

    if (availableHeight == -1)
        return availableHeight;

    availableHeight -= rootMarginBorderPaddingHeight;

    LayoutUnit result = valueForLength(height, availableHeight);
    if (includeBorderPadding) {
        result -= borderAndPaddingLogicalHeight();
        return max<LayoutUnit>(0, result);
    }
    return result;
}

LayoutUnit RenderBox::availableLogicalHeightUsing(const Length& h, AvailableLogicalHeightType heightType) const
{
    if (isRenderView())
        return isHorizontalWritingMode() ? toRenderView(this)->frameView()->visibleHeight() : toRenderView(this)->frameView()->visibleWidth();

    if (isTableCell() && (h.isAuto() || h.isPercent())) {
        if (hasOverrideHeight())
            return overrideLogicalContentHeight();
        return logicalHeight() - borderAndPaddingLogicalHeight();
    }

    if (h.isPercent() && isOutOfFlowPositioned() && !isRenderFlowThread()) {
        LayoutUnit availableHeight = containingBlockLogicalHeightForPositioned(containingBlock());
        return adjustContentBoxLogicalHeightForBoxSizing(valueForLength(h, availableHeight));
    }

    LayoutUnit heightIncludingScrollbar = computeContentAndScrollbarLogicalHeightUsing(h, -1);
    if (heightIncludingScrollbar != -1)
        return std::max<LayoutUnit>(0, adjustContentBoxLogicalHeightForBoxSizing(heightIncludingScrollbar) - scrollbarLogicalHeight());

    if (isRenderBlock() && isOutOfFlowPositioned() && style()->height().isAuto() && !(style()->top().isAuto() || style()->bottom().isAuto())) {
        RenderBlock* block = const_cast<RenderBlock*>(toRenderBlock(this));
        LogicalExtentComputedValues computedValues;
        block->computeLogicalHeight(block->logicalHeight(), 0, computedValues);
        LayoutUnit newContentHeight = computedValues.m_extent - block->borderAndPaddingLogicalHeight() - block->scrollbarLogicalHeight();
        return adjustContentBoxLogicalHeightForBoxSizing(newContentHeight);
    }

    LayoutUnit availableHeight = containingBlockLogicalHeightForContent(heightType);
    if (heightType == ExcludeMarginBorderPadding) {
        availableHeight -= marginBefore() + marginAfter() + borderAndPaddingLogicalHeight();
    }
    return availableHeight;
}

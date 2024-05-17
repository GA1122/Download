LayoutUnit RenderBox::computeReplacedLogicalHeightUsing(Length logicalHeight) const
{
    switch (logicalHeight.type()) {
        case Fixed:
            return adjustContentBoxLogicalHeightForBoxSizing(logicalHeight.value());
        case Percent:
        case Calculated:
        {
            RenderObject* cb = isOutOfFlowPositioned() ? container() : containingBlock();
            while (cb->isAnonymous())
                cb = cb->containingBlock();
            if (cb->isRenderBlock())
                toRenderBlock(cb)->addPercentHeightDescendant(const_cast<RenderBox*>(this));

            if (cb->isOutOfFlowPositioned() && cb->style()->height().isAuto() && !(cb->style()->top().isAuto() || cb->style()->bottom().isAuto())) {
                ASSERT_WITH_SECURITY_IMPLICATION(cb->isRenderBlock());
                RenderBlock* block = toRenderBlock(cb);
                LogicalExtentComputedValues computedValues;
                block->computeLogicalHeight(block->logicalHeight(), 0, computedValues);
                LayoutUnit newContentHeight = computedValues.m_extent - block->borderAndPaddingLogicalHeight() - block->scrollbarLogicalHeight();
                LayoutUnit newHeight = block->adjustContentBoxLogicalHeightForBoxSizing(newContentHeight);
                return adjustContentBoxLogicalHeightForBoxSizing(valueForLength(logicalHeight, newHeight));
            }

            LayoutUnit availableHeight;
            if (isOutOfFlowPositioned())
                availableHeight = containingBlockLogicalHeightForPositioned(toRenderBoxModelObject(cb));
            else {
                availableHeight = containingBlockLogicalHeightForContent(IncludeMarginBorderPadding);
                while (cb && !cb->isRenderView() && (cb->style()->logicalHeight().isAuto() || cb->style()->logicalHeight().isPercent())) {
                    if (cb->isTableCell()) {
                        availableHeight = max(availableHeight, intrinsicLogicalHeight());
                        return valueForLength(logicalHeight, availableHeight - borderAndPaddingLogicalHeight());
                    }
                    toRenderBlock(cb)->addPercentHeightDescendant(const_cast<RenderBox*>(this));
                    cb = cb->containingBlock();
                }
            }
            return adjustContentBoxLogicalHeightForBoxSizing(valueForLength(logicalHeight, availableHeight));
        }
        case MinContent:
        case MaxContent:
        case FitContent:
        case FillAvailable:
            return adjustContentBoxLogicalHeightForBoxSizing(computeIntrinsicLogicalContentHeightUsing(logicalHeight, intrinsicLogicalHeight(), borderAndPaddingHeight()));
        default:
            return intrinsicLogicalHeight();
    }
}

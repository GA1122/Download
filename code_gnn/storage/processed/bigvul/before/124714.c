LayoutUnit RenderBlockFlow::estimateLogicalTopPosition(RenderBox* child, const MarginInfo& marginInfo, LayoutUnit& estimateWithoutPagination)
{
    LayoutUnit logicalTopEstimate = logicalHeight();
    if (!marginInfo.canCollapseWithMarginBefore()) {
        LayoutUnit positiveMarginBefore = 0;
        LayoutUnit negativeMarginBefore = 0;
        bool discardMarginBefore = false;
        if (child->selfNeedsLayout()) {
            marginBeforeEstimateForChild(child, positiveMarginBefore, negativeMarginBefore, discardMarginBefore);
        } else {
            RenderBlockFlow::MarginValues marginValues = marginValuesForChild(child);
            positiveMarginBefore = max(positiveMarginBefore, marginValues.positiveMarginBefore());
            negativeMarginBefore = max(negativeMarginBefore, marginValues.negativeMarginBefore());
            discardMarginBefore = mustDiscardMarginBeforeForChild(child);
        }

        if (!discardMarginBefore)
            logicalTopEstimate += max(marginInfo.positiveMargin(), positiveMarginBefore) - max(marginInfo.negativeMargin(), negativeMarginBefore);
    }

    LayoutState* layoutState = view()->layoutState();
    if (layoutState->isPaginated() && layoutState->pageLogicalHeight() && logicalTopEstimate > logicalHeight())
        logicalTopEstimate = min(logicalTopEstimate, nextPageLogicalTop(logicalHeight()));

    logicalTopEstimate += getClearDelta(child, logicalTopEstimate);

    estimateWithoutPagination = logicalTopEstimate;

    if (layoutState->isPaginated()) {
        logicalTopEstimate = applyBeforeBreak(child, logicalTopEstimate);

        logicalTopEstimate = adjustForUnsplittableChild(child, logicalTopEstimate);

        if (!child->selfNeedsLayout() && child->isRenderBlock())
            logicalTopEstimate += toRenderBlock(child)->paginationStrut();
    }

    return logicalTopEstimate;
}

LayoutUnit LayoutBlockFlow::estimateLogicalTopPosition(LayoutBox& child, const MarginInfo& marginInfo, LayoutUnit& estimateWithoutPagination)
{
    LayoutUnit logicalTopEstimate = logicalHeight();
    if (!marginInfo.canCollapseWithMarginBefore()) {
        LayoutUnit positiveMarginBefore = 0;
        LayoutUnit negativeMarginBefore = 0;
        bool discardMarginBefore = false;
        if (child.selfNeedsLayout()) {
            marginBeforeEstimateForChild(child, positiveMarginBefore, negativeMarginBefore, discardMarginBefore);
        } else {
            LayoutBlockFlow::MarginValues marginValues = marginValuesForChild(child);
            positiveMarginBefore = std::max(positiveMarginBefore, marginValues.positiveMarginBefore());
            negativeMarginBefore = std::max(negativeMarginBefore, marginValues.negativeMarginBefore());
            discardMarginBefore = mustDiscardMarginBeforeForChild(child);
        }

        if (!discardMarginBefore)
            logicalTopEstimate += std::max(marginInfo.positiveMargin(), positiveMarginBefore) - std::max(marginInfo.negativeMargin(), negativeMarginBefore);
    }

    LayoutState* layoutState = view()->layoutState();
    if (layoutState->isPaginated() && isPageLogicalHeightKnown(logicalHeight()) && logicalTopEstimate > logicalHeight())
        logicalTopEstimate = std::min(logicalTopEstimate, nextPageLogicalTop(logicalHeight(), AssociateWithLatterPage));

    logicalTopEstimate += getClearDelta(&child, logicalTopEstimate);

    estimateWithoutPagination = logicalTopEstimate;

    if (layoutState->isPaginated()) {
        logicalTopEstimate = applyBeforeBreak(child, logicalTopEstimate);

        logicalTopEstimate = adjustForUnsplittableChild(child, logicalTopEstimate);

        if (!child.selfNeedsLayout() && child.isLayoutBlockFlow())
            logicalTopEstimate += toLayoutBlockFlow(&child)->paginationStrut();
    }

    return logicalTopEstimate;
}

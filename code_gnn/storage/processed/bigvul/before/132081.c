void LayoutBlockFlow::layoutBlockChild(LayoutBox& child, MarginInfo& marginInfo, LayoutUnit& previousFloatLogicalBottom)
{
    LayoutBlockFlow* childLayoutBlockFlow = child.isLayoutBlockFlow() ? toLayoutBlockFlow(&child) : nullptr;
    LayoutUnit oldPosMarginBefore = maxPositiveMarginBefore();
    LayoutUnit oldNegMarginBefore = maxNegativeMarginBefore();

    child.computeAndSetBlockDirectionMargins(this);

    LayoutUnit estimateWithoutPagination;
    LayoutUnit logicalTopEstimate = estimateLogicalTopPosition(child, marginInfo, estimateWithoutPagination);

    LayoutRect oldRect = child.frameRect();

    bool childNeededLayout = positionAndLayoutOnceIfNeeded(child, logicalTopEstimate, previousFloatLogicalBottom);

    bool atBeforeSideOfBlock = marginInfo.atBeforeSideOfBlock();
    bool childIsSelfCollapsing = child.isSelfCollapsingBlock();
    bool childDiscardMarginBefore = mustDiscardMarginBeforeForChild(child);
    bool childDiscardMarginAfter = mustDiscardMarginAfterForChild(child);

    LayoutUnit logicalTopBeforeClear = collapseMargins(child, marginInfo, childIsSelfCollapsing, childDiscardMarginBefore, childDiscardMarginAfter);

    bool childDiscardMargin = childDiscardMarginBefore || childDiscardMarginAfter;
    LayoutUnit newLogicalTop = clearFloatsIfNeeded(child, marginInfo, oldPosMarginBefore, oldNegMarginBefore, logicalTopBeforeClear, childIsSelfCollapsing, childDiscardMargin);

    bool paginated = view()->layoutState()->isPaginated();
    if (paginated) {
        if (estimateWithoutPagination != newLogicalTop) {
            positionAndLayoutOnceIfNeeded(child, newLogicalTop, previousFloatLogicalBottom);
        }

        newLogicalTop = adjustBlockChildForPagination(newLogicalTop, child, atBeforeSideOfBlock && logicalTopBeforeClear == newLogicalTop);
    }

    if (newLogicalTop != logicalTopEstimate
        || child.needsLayout()
        || (paginated && childLayoutBlockFlow && childLayoutBlockFlow->shouldBreakAtLineToAvoidWidow())) {
        positionAndLayoutOnceIfNeeded(child, newLogicalTop, previousFloatLogicalBottom);
    }

    if (!marginInfo.canCollapseMarginAfterWithLastChild() && !childIsSelfCollapsing)
        marginInfo.setCanCollapseMarginAfterWithLastChild(true);

    if (marginInfo.atBeforeSideOfBlock() && !childIsSelfCollapsing)
        marginInfo.setAtBeforeSideOfBlock(false);

    determineLogicalLeftPositionForChild(child);

    LayoutSize childOffset = child.location() - oldRect.location();

    setLogicalHeight(logicalHeight() + logicalHeightForChild(child));
    if (mustSeparateMarginAfterForChild(child)) {
        setLogicalHeight(logicalHeight() + marginAfterForChild(child));
        marginInfo.clearMargin();
    }
    if (childLayoutBlockFlow)
        addOverhangingFloats(childLayoutBlockFlow, !childNeededLayout);

    if (!selfNeedsLayout() && (childOffset.width() || childOffset.height()))
        child.invalidatePaintForOverhangingFloats(true);

    if (paginated) {
        LayoutUnit newHeight = applyAfterBreak(child, logicalHeight(), marginInfo);
        if (newHeight != size().height())
            setLogicalHeight(newHeight);
    }

    if (child.isLayoutMultiColumnSpannerPlaceholder()) {
        positionSpannerDescendant(toLayoutMultiColumnSpannerPlaceholder(child));
    }
}

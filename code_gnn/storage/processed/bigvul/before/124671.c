LayoutUnit RenderBlockFlow::adjustBlockChildForPagination(LayoutUnit logicalTopAfterClear, LayoutUnit estimateWithoutPagination, RenderBox* child, bool atBeforeSideOfBlock)
{
    RenderBlock* childRenderBlock = child->isRenderBlock() ? toRenderBlock(child) : 0;

    if (estimateWithoutPagination != logicalTopAfterClear) {
        setLogicalHeight(logicalTopAfterClear);
        setLogicalTopForChild(child, logicalTopAfterClear, ApplyLayoutDelta);

        if (child->shrinkToAvoidFloats()) {
            child->setChildNeedsLayout(MarkOnlyThis);
        }

        SubtreeLayoutScope layoutScope(*child);

        if (childRenderBlock) {
            if (!child->avoidsFloats() && childRenderBlock->containsFloats())
                toRenderBlockFlow(childRenderBlock)->markAllDescendantsWithFloatsForLayout();
            if (!child->needsLayout())
                child->markForPaginationRelayoutIfNeeded(layoutScope);
        }

        child->layoutIfNeeded();
    }

    LayoutUnit oldTop = logicalTopAfterClear;

    LayoutUnit result = applyBeforeBreak(child, logicalTopAfterClear);

    if (pageLogicalHeightForOffset(result)) {
        LayoutUnit remainingLogicalHeight = pageRemainingLogicalHeightForOffset(result, ExcludePageBoundary);
        LayoutUnit spaceShortage = child->logicalHeight() - remainingLogicalHeight;
        if (spaceShortage > 0) {
            setPageBreak(result, spaceShortage);
        }
    }

    LayoutUnit logicalTopBeforeUnsplittableAdjustment = result;
    LayoutUnit logicalTopAfterUnsplittableAdjustment = adjustForUnsplittableChild(child, result);

    LayoutUnit paginationStrut = 0;
    LayoutUnit unsplittableAdjustmentDelta = logicalTopAfterUnsplittableAdjustment - logicalTopBeforeUnsplittableAdjustment;
    if (unsplittableAdjustmentDelta)
        paginationStrut = unsplittableAdjustmentDelta;
    else if (childRenderBlock && childRenderBlock->paginationStrut())
        paginationStrut = childRenderBlock->paginationStrut();

    if (paginationStrut) {
        if (atBeforeSideOfBlock && oldTop == result && !isOutOfFlowPositioned() && !isTableCell()) {
            setPaginationStrut(result + paginationStrut);
            if (childRenderBlock)
                childRenderBlock->setPaginationStrut(0);
        } else {
            result += paginationStrut;
        }
    }

    setLogicalHeight(logicalHeight() + (result - oldTop));

    return result;
}

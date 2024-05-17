inline bool RenderBlockFlow::layoutBlockFlow(bool relayoutChildren, LayoutUnit &pageLogicalHeight, SubtreeLayoutScope& layoutScope)
{
    LayoutUnit oldLeft = logicalLeft();
    if (updateLogicalWidthAndColumnWidth())
        relayoutChildren = true;

    rebuildFloatsFromIntruding();

    bool pageLogicalHeightChanged = false;
    bool hasSpecifiedPageLogicalHeight = false;
    checkForPaginationLogicalHeightChange(pageLogicalHeight, pageLogicalHeightChanged, hasSpecifiedPageLogicalHeight);
    if (pageLogicalHeightChanged)
        relayoutChildren = true;

    LayoutStateMaintainer statePusher(*this, locationOffset(), pageLogicalHeight, pageLogicalHeightChanged, columnInfo());

    if (!isTableCell()) {
        initMaxMarginValues();
        setHasMarginBeforeQuirk(style()->hasMarginBeforeQuirk());
        setHasMarginAfterQuirk(style()->hasMarginAfterQuirk());
        setPaginationStrut(0);
    }

    LayoutUnit beforeEdge = borderBefore() + paddingBefore();
    LayoutUnit afterEdge = borderAfter() + paddingAfter() + scrollbarLogicalHeight();
    LayoutUnit previousHeight = logicalHeight();
    setLogicalHeight(beforeEdge);

    m_repaintLogicalTop = 0;
    m_repaintLogicalBottom = 0;
    if (!firstChild() && !isAnonymousBlock())
        setChildrenInline(true);

    FastTextAutosizer::LayoutScope fastTextAutosizerLayoutScope(this);

    if (childrenInline())
        layoutInlineChildren(relayoutChildren, m_repaintLogicalTop, m_repaintLogicalBottom, afterEdge);
    else
        layoutBlockChildren(relayoutChildren, layoutScope, beforeEdge, afterEdge);

    if (lowestFloatLogicalBottom() > (logicalHeight() - afterEdge) && createsBlockFormattingContext())
        setLogicalHeight(lowestFloatLogicalBottom() + afterEdge);

    if (RenderMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        if (flowThread->recalculateColumnHeights()) {
            setChildNeedsLayout(MarkOnlyThis);
            return false;
        }
    } else if (hasColumns()) {
        OwnPtr<RenderOverflow> savedOverflow = m_overflow.release();
        if (childrenInline())
            addOverflowFromInlineChildren();
        else
            addOverflowFromBlockChildren();
        LayoutUnit layoutOverflowLogicalBottom = (isHorizontalWritingMode() ? layoutOverflowRect().maxY() : layoutOverflowRect().maxX()) - borderBefore() - paddingBefore();
        m_overflow = savedOverflow.release();

        if (!hasSpecifiedPageLogicalHeight && shouldRelayoutForPagination(pageLogicalHeight, layoutOverflowLogicalBottom)) {
            setEverHadLayout(true);
            return false;
        }

        setColumnCountAndHeight(ceilf(layoutOverflowLogicalBottom.toFloat() / pageLogicalHeight.toFloat()), pageLogicalHeight.toFloat());
    }

    if (shouldBreakAtLineToAvoidWidow()) {
        setEverHadLayout(true);
        return false;
    }

    LayoutUnit oldHeight = logicalHeight();
    LayoutUnit oldClientAfterEdge = clientLogicalBottom();

    if (isRenderFlowThread())
        toRenderFlowThread(this)->applyBreakAfterContent(oldClientAfterEdge);

    updateLogicalHeight();
    LayoutUnit newHeight = logicalHeight();
    if (oldHeight > newHeight && !childrenInline()) {
        for (RenderObject* child = lastChild(); child; child = child->previousSibling()) {
            if (child->isRenderBlockFlow() && !child->isFloatingOrOutOfFlowPositioned()) {
                RenderBlockFlow* block = toRenderBlockFlow(child);
                if (block->lowestFloatLogicalBottom() + block->logicalTop() <= newHeight)
                    break;
                addOverhangingFloats(block, false);
            }
        }
    }

    bool heightChanged = (previousHeight != newHeight);
    if (heightChanged)
        relayoutChildren = true;

    layoutPositionedObjects(relayoutChildren || isDocumentElement(), oldLeft != logicalLeft() ? ForcedLayoutAfterContainingBlockMoved : DefaultLayout);

    computeRegionRangeForBlock(flowThreadContainingBlock());

    computeOverflow(oldClientAfterEdge);

    return true;
}

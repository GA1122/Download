inline bool LayoutBlockFlow::layoutBlockFlow(bool relayoutChildren, LayoutUnit &pageLogicalHeight, SubtreeLayoutScope& layoutScope)
{
    LayoutUnit oldLeft = logicalLeft();
    bool logicalWidthChanged = updateLogicalWidthAndColumnWidth();
    relayoutChildren |= logicalWidthChanged;

    rebuildFloatsFromIntruding();

    bool pageLogicalHeightChanged = false;
    bool hasSpecifiedPageLogicalHeight = false;
    checkForPaginationLogicalHeightChange(pageLogicalHeight, pageLogicalHeightChanged, hasSpecifiedPageLogicalHeight);
    if (pageLogicalHeightChanged)
        relayoutChildren = true;

    LayoutState state(*this, locationOffset(), pageLogicalHeight, pageLogicalHeightChanged, logicalWidthChanged);

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

    m_paintInvalidationLogicalTop = 0;
    m_paintInvalidationLogicalBottom = 0;
    if (!firstChild() && !isAnonymousBlock())
        setChildrenInline(true);

    TextAutosizer::LayoutScope textAutosizerLayoutScope(this);

    setContainsInlineWithOutlineAndContinuation(false);
    if (childrenInline())
        layoutInlineChildren(relayoutChildren, m_paintInvalidationLogicalTop, m_paintInvalidationLogicalBottom, afterEdge);
    else
        layoutBlockChildren(relayoutChildren, layoutScope, beforeEdge, afterEdge);

    if (needsRecalcLogicalWidthAfterLayoutChildren()) {
        clearNeedsRecalcLogicalWidthAfterLayoutChildren();
        setPreferredLogicalWidthsDirty(MarkOnlyThis);
        updateLogicalWidthAndColumnWidth();
    }

    if (lowestFloatLogicalBottom() > (logicalHeight() - afterEdge) && createsNewFormattingContext())
        setLogicalHeight(lowestFloatLogicalBottom() + afterEdge);

    if (LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        if (flowThread->recalculateColumnHeights()) {
            setChildNeedsLayout(MarkOnlyThis);
            return false;
        }
    }

    if (shouldBreakAtLineToAvoidWidow()) {
        setEverHadLayout();
        return false;
    }

    LayoutUnit oldHeight = logicalHeight();
    LayoutUnit oldClientAfterEdge = clientLogicalBottom();

    updateLogicalHeight();
    LayoutUnit newHeight = logicalHeight();
    if (!childrenInline()) {
        LayoutBlockFlow* lowestBlock = nullptr;
        bool addedOverhangingFloats = false;
        for (LayoutObject* child = lastChild(); child; child = child->previousSibling()) {
            if (child->isLayoutBlockFlow() && !child->isFloatingOrOutOfFlowPositioned()) {
                LayoutBlockFlow* block = toLayoutBlockFlow(child);
                lowestBlock = block;
                if (oldHeight <= newHeight || block->lowestFloatLogicalBottom() + block->logicalTop() <= newHeight)
                    break;
                addOverhangingFloats(block, false);
                addedOverhangingFloats = true;
            }
        }
        if (!addedOverhangingFloats)
            addLowestFloatFromChildren(lowestBlock);
    }

    bool heightChanged = (previousHeight != newHeight);
    if (heightChanged)
        relayoutChildren = true;

    layoutPositionedObjects(relayoutChildren || isDocumentElement(), oldLeft != logicalLeft() ? ForcedLayoutAfterContainingBlockMoved : DefaultLayout);

    computeOverflow(oldClientAfterEdge);

    m_descendantsWithFloatsMarkedForLayout = false;
    return true;
}

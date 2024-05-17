void LayoutBlockFlow::layoutBlockChildren(bool relayoutChildren, SubtreeLayoutScope& layoutScope, LayoutUnit beforeEdge, LayoutUnit afterEdge)
{
    dirtyForLayoutFromPercentageHeightDescendants(layoutScope);

    MarginInfo marginInfo(this, beforeEdge, afterEdge);

    LayoutObject* childToExclude = layoutSpecialExcludedChild(relayoutChildren, layoutScope);

    LayoutUnit previousFloatLogicalBottom = 0;

    LayoutBox* next = firstChildBox();
    LayoutBox* lastNormalFlowChild = nullptr;

    while (next) {
        LayoutBox* child = next;
        next = child->nextSiblingBox();

        child->setMayNeedPaintInvalidation();

        if (childToExclude == child)
            continue;  

        updateBlockChildDirtyBitsBeforeLayout(relayoutChildren, *child);

        if (child->isOutOfFlowPositioned()) {
            child->containingBlock()->insertPositionedObject(child);
            adjustPositionedBlock(*child, marginInfo);
            continue;
        }
        if (child->isFloating()) {
            insertFloatingObject(*child);
            adjustFloatingBlock(marginInfo);
            continue;
        }
        if (child->isColumnSpanAll()) {
            setLogicalHeight(logicalHeight() + marginInfo.margin());
            marginInfo.clearMargin();

            child->spannerPlaceholder()->flowThread()->skipColumnSpanner(child, offsetFromLogicalTopOfFirstPage() + logicalHeight());
            continue;
        }

        layoutBlockChild(*child, marginInfo, previousFloatLogicalBottom);
        lastNormalFlowChild = child;
    }

    handleAfterSideOfBlock(lastNormalFlowChild, beforeEdge, afterEdge, marginInfo);
}

void RenderBlockFlow::layoutBlockChildren(bool relayoutChildren, SubtreeLayoutScope& layoutScope, LayoutUnit beforeEdge, LayoutUnit afterEdge)
{
    dirtyForLayoutFromPercentageHeightDescendants(layoutScope);

    MarginInfo marginInfo(this, beforeEdge, afterEdge);

    RenderObject* childToExclude = layoutSpecialExcludedChild(relayoutChildren, layoutScope);

    LayoutUnit previousFloatLogicalBottom = 0;

    RenderBox* next = firstChildBox();
    RenderBox* lastNormalFlowChild = 0;

    while (next) {
        RenderBox* child = next;
        next = child->nextSiblingBox();

        child->setLayoutDidGetCalled(true);

        if (childToExclude == child)
            continue;  

        updateBlockChildDirtyBitsBeforeLayout(relayoutChildren, child);

        if (child->isOutOfFlowPositioned()) {
            child->containingBlock()->insertPositionedObject(child);
            adjustPositionedBlock(child, marginInfo);
            continue;
        }
        if (child->isFloating()) {
            insertFloatingObject(child);
            adjustFloatingBlock(marginInfo);
            continue;
        }

        layoutBlockChild(child, marginInfo, previousFloatLogicalBottom);
        lastNormalFlowChild = child;
    }

    handleAfterSideOfBlock(lastNormalFlowChild, beforeEdge, afterEdge, marginInfo);
}

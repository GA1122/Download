void RenderBlock::layoutPositionedObjects(bool relayoutChildren, PositionedLayoutBehavior info)
{
    TrackedRendererListHashSet* positionedDescendants = positionedObjects();
    if (!positionedDescendants)
        return;

    if (hasColumns())
        view()->layoutState()->clearPaginationInformation();  

    RenderBox* r;
    TrackedRendererListHashSet::iterator end = positionedDescendants->end();
    for (TrackedRendererListHashSet::iterator it = positionedDescendants->begin(); it != end; ++it) {
        r = *it;

        r->setLayoutDidGetCalled(true);

        SubtreeLayoutScope layoutScope(*r);
        markFixedPositionObjectForLayoutIfNeeded(r, layoutScope);
        if (info == LayoutOnlyFixedPositionedObjects) {
            r->layoutIfNeeded();
            continue;
        }

        if (relayoutChildren || (r->style()->hasStaticBlockPosition(isHorizontalWritingMode()) && r->parent() != this))
            layoutScope.setChildNeedsLayout(r);

        if (relayoutChildren && r->needsPreferredWidthsRecalculation())
            r->setPreferredLogicalWidthsDirty(MarkOnlyThis);

        if (!r->needsLayout())
            r->markForPaginationRelayoutIfNeeded(layoutScope);

        LayoutUnit oldLogicalTop = 0;
        bool needsBlockDirectionLocationSetBeforeLayout = r->needsLayout() && view()->layoutState()->needsBlockDirectionLocationSetBeforeLayout();
        if (needsBlockDirectionLocationSetBeforeLayout) {
            if (isHorizontalWritingMode() == r->isHorizontalWritingMode())
                r->updateLogicalHeight();
            else
                r->updateLogicalWidth();
            oldLogicalTop = logicalTopForChild(r);
        }

        if (info == ForcedLayoutAfterContainingBlockMoved)
            r->setNeedsLayout();

        r->layoutIfNeeded();

        if (needsBlockDirectionLocationSetBeforeLayout && logicalTopForChild(r) != oldLogicalTop)
            r->forceChildLayout();
    }

    if (hasColumns())
        view()->layoutState()->setColumnInfo(columnInfo());  
}

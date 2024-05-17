bool LayoutBlockFlow::positionAndLayoutOnceIfNeeded(LayoutBox& child, LayoutUnit newLogicalTop, LayoutUnit& previousFloatLogicalBottom)
{
    if (child.isLayoutBlockFlow()) {
        LayoutBlockFlow& childBlockFlow = toLayoutBlockFlow(child);
        if (childBlockFlow.containsFloats() || containsFloats())
            markDescendantsWithFloatsForLayoutIfNeeded(childBlockFlow, newLogicalTop, previousFloatLogicalBottom);

        if (!childBlockFlow.isWritingModeRoot())
            previousFloatLogicalBottom = std::max(previousFloatLogicalBottom, childBlockFlow.logicalTop() + childBlockFlow.lowestFloatLogicalBottom());
    }

    LayoutUnit oldLogicalTop = logicalTopForChild(child);
    setLogicalTopForChild(child, newLogicalTop);

    SubtreeLayoutScope layoutScope(child);
    if (!child.needsLayout()) {
        if (newLogicalTop != oldLogicalTop && child.shrinkToAvoidFloats()) {
            layoutScope.setChildNeedsLayout(&child);
        } else {
            child.markForPaginationRelayoutIfNeeded(layoutScope);
        }
    }

    if (!child.needsLayout())
        return false;
    child.layout();
    return true;
}

void RenderBox::repaintTreeAfterLayout()
{

    ASSERT(RuntimeEnabledFeatures::repaintAfterLayoutEnabled());
    ASSERT(!needsLayout());

    const LayoutRect oldRepaintRect = previousRepaintRect();
    setPreviousRepaintRect(clippedOverflowRectForRepaint(containerForRepaint()));

    if (view()->doingFullRepaint() && this != view()) {
        LayoutStateMaintainer statePusher(*this, isTableRow() ? LayoutSize() : locationOffset());
        RenderObject::repaintTreeAfterLayout();
        return;
    }

    if ((onlyNeededPositionedMovementLayout() && compositingState() != PaintsIntoOwnBacking)
        || (shouldDoFullRepaintIfSelfPaintingLayer()
            && hasLayer()
            && layer()->isSelfPaintingLayer())) {
        setShouldDoFullRepaintAfterLayout(true);
    }

    const LayoutRect newRepaintRect = previousRepaintRect();
    bool didFullRepaint = repaintAfterLayoutIfNeeded(containerForRepaint(),
        shouldDoFullRepaintAfterLayout(), oldRepaintRect, &newRepaintRect);

    if (!didFullRepaint)
        repaintOverflowIfNeeded();

    if (enclosingLayer()) {
        if (RenderLayerScrollableArea* area = enclosingLayer()->scrollableArea()) {
            if (area->hasVerticalBarDamage())
                repaintRectangle(area->verticalBarDamage());
            if (area->hasHorizontalBarDamage())
                repaintRectangle(area->horizontalBarDamage());
            area->resetScrollbarDamage();
        }
    }

    LayoutStateMaintainer statePusher(*this, isTableRow() ? LayoutSize() : locationOffset());
    RenderObject::repaintTreeAfterLayout();
}

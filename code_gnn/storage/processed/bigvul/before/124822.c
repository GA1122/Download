LayoutRect RenderBox::clippedOverflowRectForRepaint(const RenderLayerModelObject* repaintContainer) const
{
    if (style()->visibility() != VISIBLE && !enclosingLayer()->hasVisibleContent())
        return LayoutRect();

    LayoutRect r = visualOverflowRect();

    RenderView* v = view();
    if (!RuntimeEnabledFeatures::repaintAfterLayoutEnabled() && v) {
        r.move(v->layoutDelta());
    }

    computeRectForRepaint(repaintContainer, r);
    return r;
}

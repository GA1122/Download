bool RenderLayerScrollableArea::usesCompositedScrolling() const
{
    if (box().isIntristicallyScrollable(VerticalScrollbar) || box().isIntristicallyScrollable(HorizontalScrollbar))
        return false;

    DisableCompositingQueryAsserts disabler;
    return layer()->hasCompositedLayerMapping() && layer()->compositedLayerMapping()->scrollingLayer();
}

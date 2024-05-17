GraphicsLayer* RenderLayerScrollableArea::layerForScrollCorner() const
{
    DisableCompositingQueryAsserts disabler;

    return layer()->hasCompositedLayerMapping() ? layer()->compositedLayerMapping()->layerForScrollCorner() : 0;
}

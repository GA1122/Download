GraphicsLayer* RenderLayerScrollableArea::layerForScrolling() const
{
    return layer()->hasCompositedLayerMapping() ? layer()->compositedLayerMapping()->scrollingContentsLayer() : 0;
}

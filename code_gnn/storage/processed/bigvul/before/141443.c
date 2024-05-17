GraphicsLayer* PaintLayerScrollableArea::LayerForHorizontalScrollbar() const {
  DisableCompositingQueryAsserts disabler;

  return Layer()->HasCompositedLayerMapping()
             ? Layer()
                   ->GetCompositedLayerMapping()
                   ->LayerForHorizontalScrollbar()
             : nullptr;
}

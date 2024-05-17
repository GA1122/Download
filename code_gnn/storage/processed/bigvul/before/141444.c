GraphicsLayer* PaintLayerScrollableArea::LayerForScrollCorner() const {
  DisableCompositingQueryAsserts disabler;

  return Layer()->HasCompositedLayerMapping()
             ? Layer()->GetCompositedLayerMapping()->LayerForScrollCorner()
             : nullptr;
}

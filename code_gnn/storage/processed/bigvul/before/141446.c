GraphicsLayer* PaintLayerScrollableArea::LayerForVerticalScrollbar() const {
  DisableCompositingQueryAsserts disabler;

  return Layer()->HasCompositedLayerMapping()
             ? Layer()->GetCompositedLayerMapping()->LayerForVerticalScrollbar()
             : nullptr;
}

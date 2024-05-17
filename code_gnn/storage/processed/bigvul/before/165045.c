void HTMLCanvasElement::UnregisterContentsLayer(cc::Layer* layer) {
  GraphicsLayer::UnregisterContentsLayer(layer);
  OnContentsCcLayerChanged();
}

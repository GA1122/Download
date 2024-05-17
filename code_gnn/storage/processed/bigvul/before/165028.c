void HTMLCanvasElement::RegisterContentsLayer(cc::Layer* layer) {
  GraphicsLayer::RegisterContentsLayer(layer);
  OnContentsCcLayerChanged();
}

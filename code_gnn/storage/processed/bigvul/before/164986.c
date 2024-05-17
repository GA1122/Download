HTMLCanvasElement::CreateUnaccelerated2dBuffer() {
  auto surface = std::make_unique<Canvas2DLayerBridge>(
      Size(), Canvas2DLayerBridge::kDisableAcceleration, ColorParams());
  if (surface->IsValid())
    return surface;

  return nullptr;
}

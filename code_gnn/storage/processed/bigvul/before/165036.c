void HTMLCanvasElement::SetResourceProviderForTesting(
    std::unique_ptr<CanvasResourceProvider> resource_provider,
    std::unique_ptr<Canvas2DLayerBridge> bridge,
    const IntSize& size) {
  DiscardResourceProvider();
  SetIntegralAttribute(kWidthAttr, size.Width());
  SetIntegralAttribute(kHeightAttr, size.Height());
  SetCanvas2DLayerBridgeInternal(std::move(bridge));
  ReplaceResourceProvider(std::move(resource_provider));
}

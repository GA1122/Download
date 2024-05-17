CanvasResourceProvider* HTMLCanvasElement::GetOrCreateCanvasResourceProvider(
    AccelerationHint hint) {
  if (Is2d())
    return GetOrCreateCanvas2DLayerBridge()->GetOrCreateResourceProvider(hint);

  return CanvasRenderingContextHost::GetOrCreateCanvasResourceProvider(hint);
}

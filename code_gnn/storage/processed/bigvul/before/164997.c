CanvasRenderingContext* HTMLCanvasElement::GetCanvasRenderingContext(
    const String& type,
    const CanvasContextCreationAttributesCore& attributes) {
  auto* old_contents_cc_layer = ContentsCcLayer();
  auto* result = GetCanvasRenderingContextInternal(type, attributes);
  if (ContentsCcLayer() != old_contents_cc_layer)
    OnContentsCcLayerChanged();
  return result;
}

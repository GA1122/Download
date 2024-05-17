std::unique_ptr<TracedValue> InspectorPaintEvent::Data(
    LayoutObject* layout_object,
    const LayoutRect& clip_rect,
    const GraphicsLayer* graphics_layer) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(layout_object->GetFrame()));
  FloatQuad quad;
  LocalToPageQuad(*layout_object, clip_rect, &quad);
  CreateQuad(value.get(), "clip", quad);
  SetGeneratingNodeInfo(value.get(), layout_object, "nodeId");
  int graphics_layer_id =
      graphics_layer ? graphics_layer->PlatformLayer()->Id() : 0;
  value->SetInteger("layerId", graphics_layer_id);
  SetCallStack(value.get());
  return value;
}

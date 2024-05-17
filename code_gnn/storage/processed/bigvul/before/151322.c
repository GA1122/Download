std::unique_ptr<TracedValue> InspectorPaintImageEvent::Data(
    const LayoutObject& owning_layout_object,
    const StyleImage& style_image) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  SetGeneratingNodeInfo(value.get(), &owning_layout_object, "nodeId");
  if (const ImageResourceContent* resource = style_image.CachedImage())
    value->SetString("url", resource->Url().GetString());
  return value;
}

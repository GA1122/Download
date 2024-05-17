std::unique_ptr<TracedValue> InspectorPaintImageEvent::Data(
    const LayoutObject* owning_layout_object,
    const ImageResourceContent& image_resource) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  SetGeneratingNodeInfo(value.get(), owning_layout_object, "nodeId");
  value->SetString("url", image_resource.Url().GetString());
  return value;
}

std::unique_ptr<TracedValue> InspectorScrollLayerEvent::Data(
    LayoutObject* layout_object) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(layout_object->GetFrame()));
  SetGeneratingNodeInfo(value.get(), layout_object, "nodeId");
  return value;
}

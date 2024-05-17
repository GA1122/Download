std::unique_ptr<TracedValue> InspectorPaintInvalidationTrackingEvent::Data(
    const LayoutObject* layout_object,
    const LayoutObject& paint_container) {
  DCHECK(layout_object);
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(layout_object->GetFrame()));
  SetGeneratingNodeInfo(value.get(), &paint_container, "paintId");
  SetGeneratingNodeInfo(value.get(), layout_object, "nodeId", "nodeName");
  return value;
}

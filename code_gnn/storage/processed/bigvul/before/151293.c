std::unique_ptr<TracedValue> InspectorLayoutInvalidationTrackingEvent::Data(
    const LayoutObject* layout_object,
    LayoutInvalidationReasonForTracing reason) {
  DCHECK(layout_object);
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(layout_object->GetFrame()));
  SetGeneratingNodeInfo(value.get(), layout_object, "nodeId", "nodeName");
  value->SetString("reason", reason);
  SourceLocation::Capture()->ToTracedValue(value.get(), "stackTrace");
  return value;
}

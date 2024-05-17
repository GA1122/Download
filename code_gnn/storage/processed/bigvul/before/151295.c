std::unique_ptr<TracedValue> InspectorScrollInvalidationTrackingEvent::Data(
    const LayoutObject& layout_object) {
  static const char kScrollInvalidationReason[] =
      "Scroll with viewport-constrained element";

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(layout_object.GetFrame()));
  value->SetString("reason", kScrollInvalidationReason);
  SetGeneratingNodeInfo(value.get(), &layout_object, "nodeId", "nodeName");
  SourceLocation::Capture()->ToTracedValue(value.get(), "stackTrace");
  return value;
}

std::unique_ptr<TracedValue> InspectorRecalculateStylesEvent::Data(
    LocalFrame* frame) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("frame", ToHexString(frame));
  SetCallStack(value.get());
  return value;
}

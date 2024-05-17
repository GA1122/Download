std::unique_ptr<TracedValue> InspectorTimeStampEvent::Data(
    ExecutionContext* context,
    const String& message) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("message", message);
  if (LocalFrame* frame = FrameForExecutionContext(context))
    value->SetString("frame", ToHexString(frame));
  return value;
}

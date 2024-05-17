std::unique_ptr<TracedValue> InspectorXhrLoadEvent::Data(
    ExecutionContext* context,
    XMLHttpRequest* request) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("url", request->Url().GetString());
  if (LocalFrame* frame = FrameForExecutionContext(context))
    value->SetString("frame", ToHexString(frame));
  SetCallStack(value.get());
  return value;
}

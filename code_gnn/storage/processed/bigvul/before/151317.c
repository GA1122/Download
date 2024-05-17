std::unique_ptr<TracedValue> InspectorEvaluateScriptEvent::Data(
    LocalFrame* frame,
    const String& url,
    const TextPosition& text_position) {
  std::unique_ptr<TracedValue> value = FillLocation(url, text_position);
  value->SetString("frame", ToHexString(frame));
  SetCallStack(value.get());
  return value;
}

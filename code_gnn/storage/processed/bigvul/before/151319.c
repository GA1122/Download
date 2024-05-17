std::unique_ptr<TracedValue> InspectorCompileScriptEvent::Data(
    const String& url,
    const TextPosition& text_position) {
  return FillLocation(url, text_position);
}

std::unique_ptr<TracedValue> FillLocation(const String& url,
                                          const TextPosition& text_position) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("url", url);
  value->SetInteger("lineNumber", text_position.line_.OneBasedInt());
  value->SetInteger("columnNumber", text_position.column_.OneBasedInt());
  return value;
}

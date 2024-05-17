std::unique_ptr<TracedValue> InspectorParseHtmlEndData(unsigned end_line) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("endLine", end_line);
  return value;
}

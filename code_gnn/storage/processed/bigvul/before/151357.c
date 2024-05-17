std::unique_ptr<TracedValue> InspectorParseHtmlBeginData(Document* document,
                                                         unsigned start_line) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("startLine", start_line);
  value->SetString("frame", ToHexString(document->GetFrame()));
  value->SetString("url", document->Url().GetString());
  SetCallStack(value.get());
  return value;
}

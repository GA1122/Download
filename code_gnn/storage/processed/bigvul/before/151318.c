std::unique_ptr<TracedValue> InspectorParseScriptEvent::Data(
    unsigned long identifier,
    const String& url) {
  String request_id = IdentifiersFactory::RequestId(identifier);
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("url", url);
  return value;
}

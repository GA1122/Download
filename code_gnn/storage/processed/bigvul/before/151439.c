LoadResourceTraceData(unsigned long identifier, const KURL& url, int priority) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("url", url.GetString());
  value->SetInteger("priority", priority);
  return value;
}

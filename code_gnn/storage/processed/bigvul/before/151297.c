std::unique_ptr<TracedValue> InspectorSendRequestEvent::Data(
    unsigned long identifier,
    LocalFrame* frame,
    const ResourceRequest& request) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("frame", ToHexString(frame));
  value->SetString("url", request.Url().GetString());
  value->SetString("requestMethod", request.HttpMethod());
  const char* priority = ResourcePriorityString(request.Priority());
  if (priority)
    value->SetString("priority", priority);
  SetCallStack(value.get());
  return value;
}

std::unique_ptr<TracedValue> InspectorChangeResourcePriorityEvent::Data(
    unsigned long identifier,
    const ResourceLoadPriority& load_priority) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("priority", ResourcePriorityString(load_priority));
  return value;
}

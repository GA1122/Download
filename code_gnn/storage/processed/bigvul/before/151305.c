std::unique_ptr<TracedValue> InspectorIdleCallbackRequestEvent::Data(
    ExecutionContext* context,
    int id,
    double timeout) {
  std::unique_ptr<TracedValue> value = GenericIdleCallbackEvent(context, id);
  value->SetInteger("timeout", timeout);
  return value;
}

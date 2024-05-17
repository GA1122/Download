std::unique_ptr<TracedValue> InspectorIdleCallbackCancelEvent::Data(
    ExecutionContext* context,
    int id) {
  return GenericIdleCallbackEvent(context, id);
}

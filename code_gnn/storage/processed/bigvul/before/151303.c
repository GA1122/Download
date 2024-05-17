std::unique_ptr<TracedValue> InspectorTimerFireEvent::Data(
    ExecutionContext* context,
    int timer_id) {
  return GenericTimerData(context, timer_id);
}

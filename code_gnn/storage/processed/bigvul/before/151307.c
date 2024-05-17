std::unique_ptr<TracedValue> InspectorIdleCallbackFireEvent::Data(
    ExecutionContext* context,
    int id,
    double allotted_milliseconds,
    bool timed_out) {
  std::unique_ptr<TracedValue> value = GenericIdleCallbackEvent(context, id);
  value->SetDouble("allottedMilliseconds", allotted_milliseconds);
  value->SetBoolean("timedOut", timed_out);
  return value;
}

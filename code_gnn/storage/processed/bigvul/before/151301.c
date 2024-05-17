std::unique_ptr<TracedValue> InspectorTimerInstallEvent::Data(
    ExecutionContext* context,
    int timer_id,
    int timeout,
    bool single_shot) {
  std::unique_ptr<TracedValue> value = GenericTimerData(context, timer_id);
  value->SetInteger("timeout", timeout);
  value->SetBoolean("singleShot", single_shot);
  SetCallStack(value.get());
  return value;
}

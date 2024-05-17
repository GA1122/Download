static std::unique_ptr<TracedValue> GenericTimerData(ExecutionContext* context,
                                                     int timer_id) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("timerId", timer_id);
  if (LocalFrame* frame = FrameForExecutionContext(context))
    value->SetString("frame", ToHexString(frame));
  return value;
}

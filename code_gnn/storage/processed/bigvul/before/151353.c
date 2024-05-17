std::unique_ptr<TracedValue> GenericIdleCallbackEvent(ExecutionContext* context,
                                                      int id) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("id", id);
  if (LocalFrame* frame = FrameForExecutionContext(context))
    value->SetString("frame", ToHexString(frame));
  SetCallStack(value.get());
  return value;
}

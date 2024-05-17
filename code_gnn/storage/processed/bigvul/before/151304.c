std::unique_ptr<TracedValue> InspectorAnimationFrameEvent::Data(
    ExecutionContext* context,
    int callback_id) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("id", callback_id);
  if (context->IsDocument())
    value->SetString("frame", ToHexString(ToDocument(context)->GetFrame()));
  else if (context->IsWorkerGlobalScope())
    value->SetString("worker", ToHexString(ToWorkerGlobalScope(context)));
  SetCallStack(value.get());
  return value;
}

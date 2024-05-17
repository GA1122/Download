void DisplaySourceCustomBindings::TerminateSession(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  CHECK(args[0]->IsInt32());

  v8::Isolate* isolate = context()->isolate();
  int sink_id = args[0]->ToInt32(args.GetIsolate())->Value();
  DisplaySourceSession* session = GetDisplaySession(sink_id);
  if (!session) {
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(
        isolate, kSessionNotFound)));
    return;
  }

  DisplaySourceSession::State state = session->state();
  DCHECK_NE(state, DisplaySourceSession::Idle);
  if (state == DisplaySourceSession::Establishing) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kSessionNotFound)));
    return;
  }

  if (state == DisplaySourceSession::Terminating) {
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(
        isolate, kSessionAlreadyTerminating)));
    return;
  }

  int32_t call_id = GetCallbackId();
  args.GetReturnValue().Set(call_id);

  auto on_call_completed =
      base::Bind(&DisplaySourceCustomBindings::OnCallCompleted,
                 weak_factory_.GetWeakPtr(), call_id);
  session->Terminate(on_call_completed);
}

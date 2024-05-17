void DisplaySourceCustomBindings::DispatchSessionTerminated(int sink_id) const {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Array> event_args = v8::Array::New(isolate, 1);
  event_args->Set(0, v8::Integer::New(isolate, sink_id));
  context()->DispatchEvent("displaySource.onSessionTerminated", event_args);
}

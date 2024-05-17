void DisplaySourceCustomBindings::DispatchSessionError(
    int sink_id,
    DisplaySourceErrorType type,
    const std::string& message) const {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());

  api::display_source::ErrorInfo error_info;
  error_info.type = type;
  if (!message.empty())
    error_info.description.reset(new std::string(message));

  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  v8::Local<v8::Value> info_arg =
      converter->ToV8Value(error_info.ToValue().get(),
                           context()->v8_context());

  v8::Local<v8::Array> event_args = v8::Array::New(isolate, 2);
  event_args->Set(0, v8::Integer::New(isolate, sink_id));
  event_args->Set(1, info_arg);
  context()->DispatchEvent("displaySource.onSessionErrorOccured", event_args);
}

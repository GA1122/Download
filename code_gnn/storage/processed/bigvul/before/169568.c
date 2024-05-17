void CastStreamingNativeHandler::CallGetRawEventsCallback(
    int transport_id,
    scoped_ptr<base::BinaryValue> raw_events) {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());

  RtpStreamCallbackMap::iterator it =
      get_raw_events_callbacks_.find(transport_id);
  if (it == get_raw_events_callbacks_.end())
    return;
  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  v8::Local<v8::Value> callback_args[] = {
      converter->ToV8Value(raw_events.get(), context()->v8_context())};
  context()->CallFunction(v8::Local<v8::Function>::New(isolate, *it->second),
                          arraysize(callback_args), callback_args);
  get_raw_events_callbacks_.erase(it);
}

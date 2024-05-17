void CastStreamingNativeHandler::CallGetStatsCallback(
    int transport_id,
    scoped_ptr<base::DictionaryValue> stats) {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());

  RtpStreamCallbackMap::iterator it = get_stats_callbacks_.find(transport_id);
  if (it == get_stats_callbacks_.end())
    return;

  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  v8::Local<v8::Value> callback_args[] = {
      converter->ToV8Value(stats.get(), context()->v8_context())};
  context()->CallFunction(v8::Local<v8::Function>::New(isolate, *it->second),
                          arraysize(callback_args), callback_args);
  get_stats_callbacks_.erase(it);
}

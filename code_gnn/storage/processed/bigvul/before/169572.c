void CastStreamingNativeHandler::CallStopCallback(int stream_id) const {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Array> event_args = v8::Array::New(isolate, 1);
  event_args->Set(0, v8::Integer::New(isolate, stream_id));
  context()->DispatchEvent("cast.streaming.rtpStream.onStopped", event_args);
}

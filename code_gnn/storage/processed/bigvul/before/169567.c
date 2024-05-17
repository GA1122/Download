void CastStreamingNativeHandler::CallErrorCallback(
    int stream_id,
    const std::string& message) const {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Array> event_args = v8::Array::New(isolate, 2);
  event_args->Set(0, v8::Integer::New(isolate, stream_id));
  event_args->Set(
      1,
      v8::String::NewFromUtf8(
          isolate, message.data(), v8::String::kNormalString, message.size()));
  context()->DispatchEvent("cast.streaming.rtpStream.onError", event_args);
}

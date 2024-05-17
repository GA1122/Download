void CastStreamingNativeHandler::CallReceiverErrorCallback(
    v8::CopyablePersistentTraits<v8::Function>::CopyablePersistent function,
    const std::string& error_message) {
  v8::Isolate* isolate = context()->v8_context()->GetIsolate();
  v8::Local<v8::Value> arg = v8::String::NewFromUtf8(isolate,
                                                      error_message.data(),
                                                      v8::String::kNormalString,
                                                      error_message.size());
  context()->CallFunction(
      v8::Local<v8::Function>::New(isolate, function), 1, &arg);
}

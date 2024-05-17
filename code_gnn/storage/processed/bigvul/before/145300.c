void ObjectBackedNativeHandler::SetPrivate(v8::Local<v8::Object> obj,
                                           const char* key,
                                           v8::Local<v8::Value> value) {
  SetPrivate(context_->v8_context(), obj, key, value);
}

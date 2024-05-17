v8::Local<v8::Object> SafeBuiltins::GetString() const {
  return Load("String", context_->v8_context());
}

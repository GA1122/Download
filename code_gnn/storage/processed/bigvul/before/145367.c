v8::Local<v8::Object> SafeBuiltins::GetError() const {
  return Load("Error", context_->v8_context());
}

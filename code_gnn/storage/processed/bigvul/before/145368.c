v8::Local<v8::Object> SafeBuiltins::GetJSON() const {
  return Load("JSON", context_->v8_context());
}

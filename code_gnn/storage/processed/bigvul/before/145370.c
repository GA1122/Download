v8::Local<v8::Object> SafeBuiltins::GetRegExp() const {
  return Load("RegExp", context_->v8_context());
}

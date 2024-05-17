v8::Local<v8::Object> SafeBuiltins::GetArray() const {
  return Load("Array", context_->v8_context());
}

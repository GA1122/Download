v8::Local<v8::Object> SafeBuiltins::GetObjekt() const {
  return Load("Object", context_->v8_context());
}

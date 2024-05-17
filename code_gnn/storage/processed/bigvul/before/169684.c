bool V8ValueConverter::Strategy::FromV8Object(
    v8::Local<v8::Object> value,
    base::Value** out,
    v8::Isolate* isolate,
    const FromV8ValueCallback& callback) const {
  return false;
}

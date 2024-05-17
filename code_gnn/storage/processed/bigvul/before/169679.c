bool V8ValueConverter::Strategy::FromV8Array(
    v8::Local<v8::Array> value,
    base::Value** out,
    v8::Isolate* isolate,
    const FromV8ValueCallback& callback) const {
  return false;
}

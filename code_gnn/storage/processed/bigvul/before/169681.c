bool V8ValueConverter::Strategy::FromV8ArrayBuffer(v8::Local<v8::Object> value,
                                                   base::Value** out,
                                                   v8::Isolate* isolate) const {
  return false;
}

v8::Local<v8::Object> V8TestObject::FindInstanceInPrototypeChain(
    v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->FindInstanceInPrototypeChain(
      V8TestObject::GetWrapperTypeInfo(), v8_value);
}

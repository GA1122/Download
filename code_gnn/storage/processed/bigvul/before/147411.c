bool V8TestObject::HasInstance(v8::Local<v8::Value> v8_value, v8::Isolate* isolate) {
  return V8PerIsolateData::From(isolate)->HasInstance(V8TestObject::GetWrapperTypeInfo(), v8_value);
}

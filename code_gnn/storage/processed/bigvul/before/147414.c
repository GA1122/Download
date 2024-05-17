static void HighEntropyAttributeWithMeasureAsAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  V8SetReturnValueString(info, impl->highEntropyAttributeWithMeasureAs(), info.GetIsolate());
}

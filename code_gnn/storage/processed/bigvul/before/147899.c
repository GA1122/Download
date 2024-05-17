static void TestEnumOrDoubleAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  TestEnumOrDouble result;
  impl->testEnumOrDoubleAttribute(result);

  V8SetReturnValue(info, result);
}

static void TestInterfaceEmptyFrozenArrayAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  V8SetReturnValue(info, FreezeV8Object(ToV8(impl->testInterfaceEmptyFrozenArrayAttribute(), info.Holder(), info.GetIsolate()), info.GetIsolate()));
}

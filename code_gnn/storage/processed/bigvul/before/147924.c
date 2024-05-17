static void TestInterfaceOrLongMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceOrLong result;
  impl->testInterfaceOrLongMethod(result);
  V8SetReturnValue(info, result);
}

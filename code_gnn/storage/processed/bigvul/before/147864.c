static void StaticTestDictionaryMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestDictionary* result = TestObject::staticTestDictionaryMethod();
  V8SetReturnValue(info, result, info.GetIsolate()->GetCurrentContext()->Global());
}

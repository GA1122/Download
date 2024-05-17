static void StaticNullableTestDictionaryMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestDictionary* result = TestObject::staticNullableTestDictionaryMethod();
  V8SetReturnValue(info, result, info.GetIsolate()->GetCurrentContext()->Global());
}

static void PassPermissiveDictionaryMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "passPermissiveDictionaryMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestDictionary* arg;
  arg = NativeValueTraits<TestDictionary>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->passPermissiveDictionaryMethod(arg);
}

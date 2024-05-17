static void OverloadedMethodJ2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodJ");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestDictionary* test_dictionary_arg;
  if (!info[0]->IsNullOrUndefined() && !info[0]->IsObject()) {
    exception_state.ThrowTypeError("parameter 1 ('testDictionaryArg') is not an object.");
    return;
  }
  test_dictionary_arg = NativeValueTraits<TestDictionary>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodJ(test_dictionary_arg);
}

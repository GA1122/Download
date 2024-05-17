static void OverloadedMethodE1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodE");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t long_arg;
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodE(long_arg);
}

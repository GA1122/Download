static void DeprecateAsSameValueOverloadedMethod2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "DeprecateAsSameValueOverloadedMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t arg;
  arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->DeprecateAsSameValueOverloadedMethod(arg);
}
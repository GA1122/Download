static void OverloadedMethodF2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodF");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  double double_arg;
  double_arg = NativeValueTraits<IDLDouble>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodF(double_arg);
}

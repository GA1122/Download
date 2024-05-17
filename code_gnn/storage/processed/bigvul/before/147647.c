static void OverloadedMethodI2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodI");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  double double_arg;
  double_arg = NativeValueTraits<IDLDouble>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodI(double_arg);
}

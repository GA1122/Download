static void RaisesExceptionStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionStringMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  String result = impl->raisesExceptionStringMethod(exception_state);
  if (exception_state.HadException()) {
    return;
  }
  V8SetReturnValueString(info, result, info.GetIsolate());
}

static void RaisesExceptionVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionVoidMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  impl->raisesExceptionVoidMethod(exception_state);
  if (exception_state.HadException()) {
    return;
  }
}

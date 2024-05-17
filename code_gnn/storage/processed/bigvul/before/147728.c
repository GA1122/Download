static void RaisesExceptionTestInterfaceEmptyVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionTestInterfaceEmptyVoidMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceEmpty* result = impl->raisesExceptionTestInterfaceEmptyVoidMethod(exception_state);
  if (exception_state.HadException()) {
    return;
  }
  V8SetReturnValue(info, result);
}

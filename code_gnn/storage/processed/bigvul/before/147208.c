static void ArrayBufferViewMethodRaisesExceptionMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "arrayBufferViewMethodRaisesException");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  NotShared<TestArrayBufferView> result = impl->arrayBufferViewMethodRaisesException(exception_state);
  if (exception_state.HadException()) {
    return;
  }
  V8SetReturnValue(info, result);
}

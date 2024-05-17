static void RaisesExceptionTestInterfaceEmptyAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kGetterContext, "TestObject", "raisesExceptionTestInterfaceEmptyAttribute");

  TestInterfaceEmpty* cpp_value(impl->raisesExceptionTestInterfaceEmptyAttribute(exception_state));

  if (UNLIKELY(exception_state.HadException()))
    return;

  V8SetReturnValueFast(info, cpp_value, impl);
}

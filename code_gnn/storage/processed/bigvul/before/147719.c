static void RaisesExceptionLongAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kGetterContext, "TestObject", "raisesExceptionLongAttribute");

  int32_t cpp_value(impl->raisesExceptionLongAttribute(exception_state));

  if (UNLIKELY(exception_state.HadException()))
    return;

  V8SetReturnValueInt(info, cpp_value);
}

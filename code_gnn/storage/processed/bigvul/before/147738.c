static void RaisesExceptionXPathNSResolverVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionXPathNSResolverVoidMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  XPathNSResolver* result = impl->raisesExceptionXPathNSResolverVoidMethod(exception_state);
  if (exception_state.HadException()) {
    return;
  }
  V8SetReturnValue(info, result);
}

static void PromiseOverloadMethod1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "promiseOverloadMethod");
  ExceptionToRejectPromiseScope reject_promise_scope(info, exception_state);

  if (!V8TestObject::HasInstance(info.Holder(), info.GetIsolate())) {
    exception_state.ThrowTypeError("Illegal invocation");
    return;
  }
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValue(info, impl->promiseOverloadMethod().V8Value());
}

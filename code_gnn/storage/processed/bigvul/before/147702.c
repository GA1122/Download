static void PromiseAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kGetterContext, "TestObject", "promiseAttribute");
  ExceptionToRejectPromiseScope reject_promise_scope(info, exception_state);

  if (!V8TestObject::HasInstance(info.Holder(), info.GetIsolate())) {
    exception_state.ThrowTypeError("Illegal invocation");
    return;
  }

  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  V8SetReturnValue(info, impl->promiseAttribute().V8Value());
}

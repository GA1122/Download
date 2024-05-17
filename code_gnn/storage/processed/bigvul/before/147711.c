static void PromiseOverloadMethod2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "promiseOverloadMethod");
  ExceptionToRejectPromiseScope reject_promise_scope(info, exception_state);

  if (!V8TestObject::HasInstance(info.Holder(), info.GetIsolate())) {
    exception_state.ThrowTypeError("Illegal invocation");
    return;
  }
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  DOMWindow* arg_1;
  double arg_2;
  arg_1 = ToDOMWindow(info.GetIsolate(), info[0]);
  if (!arg_1) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "Window"));
    return;
  }

  arg_2 = NativeValueTraits<IDLDouble>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  V8SetReturnValue(info, impl->promiseOverloadMethod(arg_1, arg_2).V8Value());
}

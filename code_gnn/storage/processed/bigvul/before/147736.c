static void RaisesExceptionVoidMethodTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionVoidMethodTestCallbackInterfaceArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  V8TestCallbackInterface* test_callback_interface_arg;
  if (info[0]->IsObject()) {
    test_callback_interface_arg = V8TestCallbackInterface::Create(info[0].As<v8::Object>());
  } else {
    exception_state.ThrowTypeError("The callback provided as parameter 1 is not an object.");
    return;
  }

  impl->raisesExceptionVoidMethodTestCallbackInterfaceArg(test_callback_interface_arg, exception_state);
  if (exception_state.HadException()) {
    return;
  }
}

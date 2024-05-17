static void RaisesExceptionVoidMethodOptionalTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "raisesExceptionVoidMethodOptionalTestCallbackInterfaceArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8TestCallbackInterface* optional_test_callback_interface_arg;
  if (info[0]->IsObject()) {
    optional_test_callback_interface_arg = V8TestCallbackInterface::Create(info[0].As<v8::Object>());
  } else if (info[0]->IsUndefined()) {
    optional_test_callback_interface_arg = nullptr;
  } else {
    exception_state.ThrowTypeError("The callback provided as parameter 1 is not an object.");
    return;
  }

  impl->raisesExceptionVoidMethodOptionalTestCallbackInterfaceArg(optional_test_callback_interface_arg, exception_state);
  if (exception_state.HadException()) {
    return;
  }
}

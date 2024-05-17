static void VoidMethodOptionalTestCallbackInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8TestCallbackInterface* optional_test_callback_interface_arg;
  if (info[0]->IsObject()) {
    optional_test_callback_interface_arg = V8TestCallbackInterface::Create(info[0].As<v8::Object>());
  } else if (info[0]->IsUndefined()) {
    optional_test_callback_interface_arg = nullptr;
  } else {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodOptionalTestCallbackInterfaceArg", "TestObject", "The callback provided as parameter 1 is not an object."));
    return;
  }

  impl->voidMethodOptionalTestCallbackInterfaceArg(optional_test_callback_interface_arg);
}

static void VoidMethodTestCallbackInterfaceOrNullArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodTestCallbackInterfaceOrNullArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  V8TestCallbackInterface* test_callback_interface_arg;
  if (info[0]->IsObject()) {
    test_callback_interface_arg = V8TestCallbackInterface::Create(info[0].As<v8::Object>());
  } else if (info[0]->IsNullOrUndefined()) {
    test_callback_interface_arg = nullptr;
  } else {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodTestCallbackInterfaceOrNullArg", "TestObject", "The callback provided as parameter 1 is not an object."));
    return;
  }

  impl->voidMethodTestCallbackInterfaceOrNullArg(test_callback_interface_arg);
}

static void OverloadedMethodN2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8TestCallbackInterface* test_callback_interface_arg;
  if (info[0]->IsObject()) {
    test_callback_interface_arg = V8TestCallbackInterface::Create(info[0].As<v8::Object>());
  } else {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("overloadedMethodN", "TestObject", "The callback provided as parameter 1 is not an object."));
    return;
  }

  impl->overloadedMethodN(test_callback_interface_arg);
}

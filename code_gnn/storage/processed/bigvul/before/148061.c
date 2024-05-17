static void VoidMethodDefaultNullableTestInterfaceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceImplementation* default_test_interface_arg;
  if (!info[0]->IsUndefined()) {
    default_test_interface_arg = V8TestInterface::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
    if (!default_test_interface_arg && !IsUndefinedOrNull(info[0])) {
      V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDefaultNullableTestInterfaceArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterface")));
      return;
    }
  } else {
    default_test_interface_arg = nullptr;
  }

  impl->voidMethodDefaultNullableTestInterfaceArg(default_test_interface_arg);
}

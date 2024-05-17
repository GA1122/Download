static void VoidMethodDefaultUndefinedTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceEmpty* default_undefined_test_interface_empty_arg;
  default_undefined_test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!default_undefined_test_interface_empty_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodDefaultUndefinedTestInterfaceEmptyArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty")));
    return;
  }

  impl->voidMethodDefaultUndefinedTestInterfaceEmptyArg(default_undefined_test_interface_empty_arg);
}

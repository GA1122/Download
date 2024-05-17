static void PerWorldBindingsVoidMethodTestInterfaceEmptyArgMethodForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("perWorldBindingsVoidMethodTestInterfaceEmptyArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  TestInterfaceEmpty* test_interface_empty_arg;
  test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!test_interface_empty_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("perWorldBindingsVoidMethodTestInterfaceEmptyArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty")));
    return;
  }

  impl->perWorldBindingsVoidMethodTestInterfaceEmptyArg(test_interface_empty_arg);
}

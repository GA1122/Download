static void OverloadedMethodN1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceImplementation* test_interface_arg;
  test_interface_arg = V8TestInterface::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!test_interface_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("overloadedMethodN", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterface")));
    return;
  }

  impl->overloadedMethodN(test_interface_arg);
}

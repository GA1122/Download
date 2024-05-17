static void OverloadedMethodC2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceEmpty* test_interface_empty_arg;
  test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!test_interface_empty_arg) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("overloadedMethodC", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty")));
    return;
  }

  impl->overloadedMethodC(test_interface_empty_arg);
}

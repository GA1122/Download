static void OverloadedMethodG2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  TestInterfaceEmpty* test_interface_empty_or_null_arg;
  if (!info[0]->IsUndefined()) {
    test_interface_empty_or_null_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
    if (!test_interface_empty_or_null_arg && !IsUndefinedOrNull(info[0])) {
      V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("overloadedMethodG", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty")));
      return;
    }
  } else {
    test_interface_empty_or_null_arg = nullptr;
  }

  impl->overloadedMethodG(test_interface_empty_or_null_arg);
}

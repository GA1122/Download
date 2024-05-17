static void VoidMethodTestInterfaceEmptyOrNullArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodTestInterfaceEmptyOrNullArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  TestInterfaceEmpty* nullable_test_interface_empty_arg;
  nullable_test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!nullable_test_interface_empty_arg && !IsUndefinedOrNull(info[0])) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodTestInterfaceEmptyOrNullArg", "TestObject", ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty")));
    return;
  }

  impl->voidMethodTestInterfaceEmptyOrNullArg(nullable_test_interface_empty_arg);
}

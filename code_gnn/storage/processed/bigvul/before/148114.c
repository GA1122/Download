static void VoidMethodLongArgOptionalTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodLongArgOptionalTestInterfaceEmptyArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  int32_t long_arg;
  TestInterfaceEmpty* optional_test_interface_empty;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  if (UNLIKELY(num_args_passed <= 1)) {
    impl->voidMethodLongArgOptionalTestInterfaceEmptyArg(long_arg);
    return;
  }
  optional_test_interface_empty = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[1]);
  if (!optional_test_interface_empty) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(1, "TestInterfaceEmpty"));
    return;
  }

  impl->voidMethodLongArgOptionalTestInterfaceEmptyArg(long_arg, optional_test_interface_empty);
}

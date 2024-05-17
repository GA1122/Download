static void VoidMethodLongArgOptionalLongArgOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodLongArgOptionalLongArgOptionalLongArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  int32_t long_arg;
  int32_t optional_long_arg_1;
  int32_t optional_long_arg_2;
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
    impl->voidMethodLongArgOptionalLongArgOptionalLongArg(long_arg);
    return;
  }
  optional_long_arg_1 = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  if (UNLIKELY(num_args_passed <= 2)) {
    impl->voidMethodLongArgOptionalLongArgOptionalLongArg(long_arg, optional_long_arg_1);
    return;
  }
  optional_long_arg_2 = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[2], exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodLongArgOptionalLongArgOptionalLongArg(long_arg, optional_long_arg_1, optional_long_arg_2);
}

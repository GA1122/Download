static void VoidMethodByteStringOrNullOptionalUSVStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodByteStringOrNullOptionalUSVStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  V8StringResource<kTreatNullAndUndefinedAsNullString> byte_string_arg;
  V8StringResource<> usv_string_arg;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  byte_string_arg = NativeValueTraits<IDLByteStringOrNull>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  if (UNLIKELY(num_args_passed <= 1)) {
    impl->voidMethodByteStringOrNullOptionalUSVStringArg(byte_string_arg);
    return;
  }
  usv_string_arg = NativeValueTraits<IDLUSVString>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodByteStringOrNullOptionalUSVStringArg(byte_string_arg, usv_string_arg);
}

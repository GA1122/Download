static void VoidMethodTreatNullAsEmptyStringStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    V8ThrowException::ThrowTypeError(info.GetIsolate(), ExceptionMessages::FailedToExecute("voidMethodTreatNullAsEmptyStringStringArg", "TestObject", ExceptionMessages::NotEnoughArguments(1, info.Length())));
    return;
  }

  V8StringResource<kTreatNullAsEmptyString> treat_null_as_empty_string_string_arg;
  treat_null_as_empty_string_string_arg = info[0];
  if (!treat_null_as_empty_string_string_arg.Prepare())
    return;

  impl->voidMethodTreatNullAsEmptyStringStringArg(treat_null_as_empty_string_string_arg);
}

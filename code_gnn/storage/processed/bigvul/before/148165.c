static void VoidMethodTestEnumArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodTestEnumArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  V8StringResource<> test_enum_type_arg;
  test_enum_type_arg = info[0];
  if (!test_enum_type_arg.Prepare())
    return;
  const char* const kValidTestEnumTypeArgValues[] = {
      "",
      "EnumValue1",
      "EnumValue2",
      "EnumValue3",
  };
  if (!IsValidEnum(test_enum_type_arg, kValidTestEnumTypeArgValues, base::size(kValidTestEnumTypeArgValues), "TestEnum", exception_state)) {
    return;
  }

  impl->voidMethodTestEnumArg(test_enum_type_arg);
}

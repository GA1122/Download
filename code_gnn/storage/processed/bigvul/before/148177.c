static void VoidMethodTestMultipleEnumArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodTestMultipleEnumArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 2)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(2, info.Length()));
    return;
  }

  V8StringResource<> test_enum_type_arg;
  V8StringResource<> test_enum_type_arg_2;
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

  test_enum_type_arg_2 = info[1];
  if (!test_enum_type_arg_2.Prepare())
    return;
  const char* const kValidTestEnumTypeArg2Values[] = {
      "",
      "EnumValue1",
      "EnumValue2",
      "EnumValue3",
  };
  if (!IsValidEnum(test_enum_type_arg_2, kValidTestEnumTypeArg2Values, base::size(kValidTestEnumTypeArg2Values), "TestEnum2", exception_state)) {
    return;
  }

  impl->voidMethodTestMultipleEnumArg(test_enum_type_arg, test_enum_type_arg_2);
}

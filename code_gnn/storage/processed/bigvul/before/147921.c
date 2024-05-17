static void TestInterfaceEmptyMethodOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "testInterfaceEmptyMethodOptionalLongArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t optional_long_arg;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  if (UNLIKELY(num_args_passed <= 0)) {
    V8SetReturnValue(info, impl->testInterfaceEmptyMethodOptionalLongArg());
    return;
  }
  optional_long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  V8SetReturnValue(info, impl->testInterfaceEmptyMethodOptionalLongArg(optional_long_arg));
}

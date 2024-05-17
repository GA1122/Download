static void StringMethodOptionalLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "stringMethodOptionalLongArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t optional_long_arg;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  if (UNLIKELY(num_args_passed <= 0)) {
    V8SetReturnValueString(info, impl->stringMethodOptionalLongArg(), info.GetIsolate());
    return;
  }
  optional_long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  V8SetReturnValueString(info, impl->stringMethodOptionalLongArg(optional_long_arg), info.GetIsolate());
}

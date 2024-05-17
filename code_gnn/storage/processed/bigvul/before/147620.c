static void OverloadedMethodB2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodB");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> string_arg;
  int32_t long_arg;
  int num_args_passed = info.Length();
  while (num_args_passed > 0) {
    if (!info[num_args_passed - 1]->IsUndefined())
      break;
    --num_args_passed;
  }
  string_arg = info[0];
  if (!string_arg.Prepare())
    return;

  if (UNLIKELY(num_args_passed <= 1)) {
    impl->overloadedMethodB(string_arg);
    return;
  }
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodB(string_arg, long_arg);
}

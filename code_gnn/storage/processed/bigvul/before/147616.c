static void OverloadedMethodA2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodA");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t long_arg_1;
  int32_t long_arg_2;
  long_arg_1 = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  long_arg_2 = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[1], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodA(long_arg_1, long_arg_2);
}

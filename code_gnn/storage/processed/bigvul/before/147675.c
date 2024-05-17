static void PartiallyRuntimeEnabledOverloadedVoidMethod3Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "partiallyRuntimeEnabledOverloadedVoidMethod");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t long_arg;
  V8StringResource<> string_arg;
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  string_arg = info[1];
  if (!string_arg.Prepare())
    return;

  impl->partiallyRuntimeEnabledOverloadedVoidMethod(long_arg, string_arg);
}

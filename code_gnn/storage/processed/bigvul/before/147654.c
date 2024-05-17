static void OverloadedMethodL1Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodL");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t long_arg;
  HeapVector<ScriptValue> rest_args;
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  rest_args = ToImplArguments<ScriptValue>(info, 1, exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodL(long_arg, rest_args);
}

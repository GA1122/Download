static void OverloadedMethodL2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodL");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> string_arg;
  HeapVector<ScriptValue> rest_args;
  string_arg = info[0];
  if (!string_arg.Prepare())
    return;

  rest_args = ToImplArguments<ScriptValue>(info, 1, exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodL(string_arg, rest_args);
}

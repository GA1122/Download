static void VoidMethodVariadicStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodVariadicStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  Vector<String> variadic_string_args;
  variadic_string_args = ToImplArguments<IDLString>(info, 0, exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodVariadicStringArg(variadic_string_args);
}

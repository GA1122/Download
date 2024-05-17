static void VoidMethodStringArgVariadicStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodStringArgVariadicStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  V8StringResource<> string_arg;
  Vector<String> variadic_string_args;
  string_arg = info[0];
  if (!string_arg.Prepare())
    return;

  variadic_string_args = ToImplArguments<IDLString>(info, 1, exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodStringArgVariadicStringArg(string_arg, variadic_string_args);
}

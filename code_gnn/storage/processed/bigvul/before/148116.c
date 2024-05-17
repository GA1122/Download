static void VoidMethodLongArgTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodLongArgTestInterfaceEmptyArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 2)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(2, info.Length()));
    return;
  }

  int32_t long_arg;
  TestInterfaceEmpty* test_interface_empty_arg;
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[1]);
  if (!test_interface_empty_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(1, "TestInterfaceEmpty"));
    return;
  }

  impl->voidMethodLongArgTestInterfaceEmptyArg(long_arg, test_interface_empty_arg);
}

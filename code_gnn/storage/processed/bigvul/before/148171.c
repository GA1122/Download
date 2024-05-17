static void VoidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  TestInterfaceEmpty* test_interface_empty_arg;
  HeapVector<Member<TestInterfaceEmpty>> variadic_test_interface_empty_args;
  test_interface_empty_arg = V8TestInterfaceEmpty::ToImplWithTypeCheck(info.GetIsolate(), info[0]);
  if (!test_interface_empty_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty"));
    return;
  }

  for (int i = 1; i < info.Length(); ++i) {
    if (!V8TestInterfaceEmpty::HasInstance(info[i], info.GetIsolate())) {
      exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(1, "TestInterfaceEmpty"));
      return;
    }
    variadic_test_interface_empty_args.push_back(V8TestInterfaceEmpty::ToImpl(v8::Local<v8::Object>::Cast(info[i])));
  }

  impl->voidMethodTestInterfaceEmptyArgVariadicTestInterfaceEmptyArg(test_interface_empty_arg, variadic_test_interface_empty_args);
}

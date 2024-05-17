static void VoidMethodVariadicTestInterfaceEmptyArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodVariadicTestInterfaceEmptyArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  HeapVector<Member<TestInterfaceEmpty>> variadic_test_interface_empty_args;
  for (int i = 0; i < info.Length(); ++i) {
    if (!V8TestInterfaceEmpty::HasInstance(info[i], info.GetIsolate())) {
      exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "TestInterfaceEmpty"));
      return;
    }
    variadic_test_interface_empty_args.push_back(V8TestInterfaceEmpty::ToImpl(v8::Local<v8::Object>::Cast(info[i])));
  }

  impl->voidMethodVariadicTestInterfaceEmptyArg(variadic_test_interface_empty_args);
}

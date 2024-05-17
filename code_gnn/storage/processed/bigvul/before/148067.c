static void VoidMethodDefaultTrueBooleanArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultTrueBooleanArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  bool default_boolean_arg;
  if (!info[0]->IsUndefined()) {
    default_boolean_arg = NativeValueTraits<IDLBoolean>::NativeValue(info.GetIsolate(), info[0], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_boolean_arg = true;
  }

  impl->voidMethodDefaultTrueBooleanArg(default_boolean_arg);
}

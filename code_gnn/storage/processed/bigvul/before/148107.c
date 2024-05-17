static void VoidMethodInt32ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodInt32ArrayArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  NotShared<DOMInt32Array> int32_array_arg;
  int32_array_arg = ToNotShared<NotShared<DOMInt32Array>>(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;
  if (!int32_array_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "Int32Array"));
    return;
  }

  impl->voidMethodInt32ArrayArg(int32_array_arg);
}

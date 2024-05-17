static void VoidMethodAllowSharedUint8ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodAllowSharedUint8ArrayArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  MaybeShared<DOMUint8Array> uint8_array_arg;
  uint8_array_arg = ToMaybeShared<MaybeShared<DOMUint8Array>>(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;
  if (!uint8_array_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "Uint8Array"));
    return;
  }

  impl->voidMethodAllowSharedUint8ArrayArg(uint8_array_arg);
}

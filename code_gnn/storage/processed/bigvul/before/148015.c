static void VoidMethodAllowSharedArrayBufferViewArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodAllowSharedArrayBufferViewArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  MaybeShared<TestArrayBufferView> array_buffer_view_arg;
  array_buffer_view_arg = ToMaybeShared<MaybeShared<TestArrayBufferView>>(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;
  if (!array_buffer_view_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "ArrayBufferView"));
    return;
  }

  impl->voidMethodAllowSharedArrayBufferViewArg(array_buffer_view_arg);
}

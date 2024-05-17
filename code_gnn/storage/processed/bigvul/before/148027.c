static void VoidMethodArrayBufferViewArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodArrayBufferViewArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  NotShared<TestArrayBufferView> array_buffer_view_arg;
  array_buffer_view_arg = ToNotShared<NotShared<TestArrayBufferView>>(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;
  if (!array_buffer_view_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "ArrayBufferView"));
    return;
  }

  impl->voidMethodArrayBufferViewArg(array_buffer_view_arg);
}

static void VoidMethodFlexibleArrayBufferViewArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodFlexibleArrayBufferViewArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  FlexibleArrayBufferView array_buffer_view_arg;
  ToFlexibleArrayBufferView(info.GetIsolate(), info[0], array_buffer_view_arg, allocateFlexibleArrayBufferViewStorage(info[0]));
  if (!array_buffer_view_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "ArrayBufferView"));
    return;
  }

  impl->voidMethodFlexibleArrayBufferViewArg(array_buffer_view_arg);
}

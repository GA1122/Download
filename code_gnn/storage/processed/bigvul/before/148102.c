static void VoidMethodFlexibleArrayBufferViewTypedArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodFlexibleArrayBufferViewTypedArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  FlexibleFloat32ArrayView typed_array_buffer_view_arg;
  ToFlexibleArrayBufferView(info.GetIsolate(), info[0], typed_array_buffer_view_arg, allocateFlexibleArrayBufferViewStorage(info[0]));
  if (!typed_array_buffer_view_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "Float32Array"));
    return;
  }

  impl->voidMethodFlexibleArrayBufferViewTypedArg(typed_array_buffer_view_arg);
}

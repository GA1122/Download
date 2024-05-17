static void VoidMethodFloat32ArrayArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodFloat32ArrayArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  NotShared<DOMFloat32Array> float32_array_arg;
  float32_array_arg = ToNotShared<NotShared<DOMFloat32Array>>(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;
  if (!float32_array_arg) {
    exception_state.ThrowTypeError(ExceptionMessages::ArgumentNotOfType(0, "Float32Array"));
    return;
  }

  impl->voidMethodFloat32ArrayArg(float32_array_arg);
}

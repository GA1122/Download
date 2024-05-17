static void VoidMethodDefaultIntegerArgsMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultIntegerArgs");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  int32_t default_long_arg;
  int64_t default_long_long_arg;
  uint32_t default_unsigned_arg;
  if (!info[0]->IsUndefined()) {
    default_long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_long_arg = 10;
  }
  if (!info[1]->IsUndefined()) {
    default_long_long_arg = NativeValueTraits<IDLLongLong>::NativeValue(info.GetIsolate(), info[1], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_long_long_arg = -10;
  }
  if (!info[2]->IsUndefined()) {
    default_unsigned_arg = NativeValueTraits<IDLUnsignedLong>::NativeValue(info.GetIsolate(), info[2], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_unsigned_arg = 4294967295u;
  }

  impl->voidMethodDefaultIntegerArgs(default_long_arg, default_long_long_arg, default_unsigned_arg);
}

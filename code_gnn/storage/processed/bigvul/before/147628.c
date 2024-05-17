static void OverloadedMethodD2Method(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "overloadedMethodD");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  Vector<int32_t> long_array_sequence;
  long_array_sequence = NativeValueTraits<IDLSequence<IDLLong>>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->overloadedMethodD(long_array_sequence);
}

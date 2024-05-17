static void CallWithExecutionContextRaisesExceptionVoidMethodLongArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "callWithExecutionContextRaisesExceptionVoidMethodLongArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  int32_t long_arg;
  long_arg = NativeValueTraits<IDLLong>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  ExecutionContext* execution_context = ExecutionContext::ForRelevantRealm(info);
  impl->callWithExecutionContextRaisesExceptionVoidMethodLongArg(execution_context, long_arg, exception_state);
  if (exception_state.HadException()) {
    return;
  }
}

static void VoidMethodDefaultStringSequenceArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultStringSequenceArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  Vector<String> default_string_sequence_arg;
  if (!info[0]->IsUndefined()) {
    default_string_sequence_arg = NativeValueTraits<IDLSequence<IDLString>>::NativeValue(info.GetIsolate(), info[0], exception_state);
    if (exception_state.HadException())
      return;
  } else {
     ;
  }

  impl->voidMethodDefaultStringSequenceArg(default_string_sequence_arg);
}

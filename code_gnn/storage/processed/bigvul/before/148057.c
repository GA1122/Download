static void VoidMethodDefaultNullableByteStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultNullableByteStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<kTreatNullAndUndefinedAsNullString> default_string_arg;
  if (!info[0]->IsUndefined()) {
    default_string_arg = NativeValueTraits<IDLByteStringOrNull>::NativeValue(info.GetIsolate(), info[0], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_string_arg = nullptr;
  }

  impl->voidMethodDefaultNullableByteStringArg(default_string_arg);
}

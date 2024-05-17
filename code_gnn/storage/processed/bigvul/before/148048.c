static void VoidMethodDefaultByteStringArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodDefaultByteStringArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8StringResource<> default_byte_string_arg;
  if (!info[0]->IsUndefined()) {
    default_byte_string_arg = NativeValueTraits<IDLByteString>::NativeValue(info.GetIsolate(), info[0], exception_state);
    if (exception_state.HadException())
      return;
  } else {
    default_byte_string_arg = "foo";
  }

  impl->voidMethodDefaultByteStringArg(default_byte_string_arg);
}

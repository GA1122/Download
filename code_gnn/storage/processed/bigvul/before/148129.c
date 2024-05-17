static void VoidMethodOptionalDictionaryArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodOptionalDictionaryArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  Dictionary optional_dictionary_arg;
  if (!info[0]->IsNullOrUndefined() && !info[0]->IsObject()) {
    exception_state.ThrowTypeError("parameter 1 ('optionalDictionaryArg') is not an object.");
    return;
  }
  optional_dictionary_arg = NativeValueTraits<Dictionary>::NativeValue(info.GetIsolate(), info[0], exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodOptionalDictionaryArg(optional_dictionary_arg);
}

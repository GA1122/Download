static void VoidMethodArrayBufferOrArrayBufferViewOrDictionaryArgMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  ExceptionState exception_state(info.GetIsolate(), ExceptionState::kExecutionContext, "TestObject", "voidMethodArrayBufferOrArrayBufferViewOrDictionaryArg");

  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  if (UNLIKELY(info.Length() < 1)) {
    exception_state.ThrowTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
    return;
  }

  ArrayBufferOrArrayBufferViewOrDictionary arg;
  V8ArrayBufferOrArrayBufferViewOrDictionary::ToImpl(info.GetIsolate(), info[0], arg, UnionTypeConversionMode::kNotNullable, exception_state);
  if (exception_state.HadException())
    return;

  impl->voidMethodArrayBufferOrArrayBufferViewOrDictionaryArg(arg);
}

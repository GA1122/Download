static void NullableLongMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  base::Optional<int32_t> result = impl->nullableLongMethod();
  if (!result)
    V8SetReturnValueNull(info);
  else
    V8SetReturnValueInt(info, result.value());
}

static void NullableStringMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValueStringOrNull(info, impl->nullableStringMethod(), info.GetIsolate());
}

static void UnsignedShortMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValueUnsigned(info, impl->unsignedShortMethod());
}

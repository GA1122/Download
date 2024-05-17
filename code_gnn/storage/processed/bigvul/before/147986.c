static void UnsignedLongLongMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValue(info, static_cast<double>(impl->unsignedLongLongMethod()));
}

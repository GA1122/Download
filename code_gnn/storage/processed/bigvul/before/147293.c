static void CustomCallEpilogueVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  impl->customCallEpilogueVoidMethod();
  V8TestObject::CustomCallEpilogueVoidMethodMethodEpilogueCustom(info, impl);
}

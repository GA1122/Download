static void CustomCallPrologueVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8TestObject::CustomCallPrologueVoidMethodMethodPrologueCustom(info, impl);
  impl->customCallPrologueVoidMethod();
}

static void NodeFilterMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValue(info, ToV8(impl->nodeFilterMethod(), info.Holder(), info.GetIsolate()));
}

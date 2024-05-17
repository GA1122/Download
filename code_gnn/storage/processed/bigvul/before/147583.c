static void NodeFilterAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  V8SetReturnValue(info, ToV8(WTF::GetPtr(impl->nodeFilterAttribute()), info.Holder(), info.GetIsolate()));
}

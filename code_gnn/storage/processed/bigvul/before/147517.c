static void LongFrozenArrayMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValue(info, FreezeV8Object(ToV8(impl->longFrozenArrayMethod(), info.Holder(), info.GetIsolate()), info.GetIsolate()));
}

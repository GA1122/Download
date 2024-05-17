static void DoubleOrNullStringAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  DoubleOrString result;
  impl->doubleOrNullStringAttribute(result);

  V8SetReturnValue(info, result);
}

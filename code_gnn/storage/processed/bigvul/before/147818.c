static void SaveSameObjectAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  static const V8PrivateProperty::SymbolKey kSaveSameObjectKey;
  auto private_same_object =
      V8PrivateProperty::GetSymbol(info.GetIsolate(), kSaveSameObjectKey);
  {
    v8::Local<v8::Value> v8_value;
    if (private_same_object.GetOrUndefined(holder).ToLocal(&v8_value) && !v8_value->IsUndefined()) {
      V8SetReturnValue(info, v8_value);
      return;
    }
  }

  TestObject* impl = V8TestObject::ToImpl(holder);

  TestInterfaceImplementation* cpp_value(WTF::GetPtr(impl->saveSameObjectAttribute()));

  if (cpp_value && DOMDataStore::SetReturnValue(info.GetReturnValue(), cpp_value))
    return;
  v8::Local<v8::Value> v8_value(ToV8(cpp_value, holder, info.GetIsolate()));
  static const V8PrivateProperty::SymbolKey kKeepAliveKey;
  V8PrivateProperty::GetSymbol(info.GetIsolate(), kKeepAliveKey)
      .Set(holder, v8_value);

  V8SetReturnValue(info, v8_value);

  private_same_object.Set(holder, info.GetReturnValue().Get());
}
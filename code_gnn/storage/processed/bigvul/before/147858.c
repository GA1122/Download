static void StaticSaveSameObjectAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
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

  V8SetReturnValue(info, WTF::GetPtr(TestObject::staticSaveSameObjectAttribute()), info.GetIsolate()->GetCurrentContext()->Global());

  private_same_object.Set(holder, info.GetReturnValue().Get());
}

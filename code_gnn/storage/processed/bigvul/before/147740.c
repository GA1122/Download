static void ReadonlyCachedAttributeAttributeGetter(const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> holder = info.Holder();

  TestObject* impl = V8TestObject::ToImpl(holder);

  static const V8PrivateProperty::SymbolKey kPrivatePropertyReadonlyCachedAttribute;

  V8PrivateProperty::Symbol property_symbol =
      V8PrivateProperty::GetSymbol(info.GetIsolate(),
          kPrivatePropertyReadonlyCachedAttribute);
  if (!static_cast<const TestObject*>(impl)->isStringDirty()) {
    v8::Local<v8::Value> v8_value;
    if (property_symbol.GetOrUndefined(holder).ToLocal(&v8_value) && !v8_value->IsUndefined()) {
      V8SetReturnValue(info, v8_value);
      return;
    }
  }

  String cpp_value(impl->readonlyCachedAttribute());

  v8::Local<v8::Value> v8_value(V8String(info.GetIsolate(), cpp_value));
  property_symbol.Set(holder, v8_value);

  V8SetReturnValue(info, v8_value);
}

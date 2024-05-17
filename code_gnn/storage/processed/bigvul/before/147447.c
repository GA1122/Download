static void IndexedPropertyDescriptor(
    uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info) {
  V8TestObject::IndexedPropertyGetterCallback(index, info);
  v8::Local<v8::Value> getter_value = info.GetReturnValue().Get();
  if (!getter_value->IsUndefined()) {
    v8::PropertyDescriptor desc(getter_value, true);
    desc.set_enumerable(true);
    desc.set_configurable(true);
    V8SetReturnValue(info, desc);
  }
}

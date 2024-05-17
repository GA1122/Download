static void SerializedScriptValueAttributeAttributeSetter(
    v8::Local<v8::Value> v8_value, const v8::FunctionCallbackInfo<v8::Value>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  ALLOW_UNUSED_LOCAL(isolate);

  v8::Local<v8::Object> holder = info.Holder();
  ALLOW_UNUSED_LOCAL(holder);

  TestObject* impl = V8TestObject::ToImpl(holder);

  ExceptionState exception_state(isolate, ExceptionState::kSetterContext, "TestObject", "serializedScriptValueAttribute");

  scoped_refptr<SerializedScriptValue> cpp_value = NativeValueTraits<SerializedScriptValue>::NativeValue(info.GetIsolate(), v8_value, SerializedScriptValue::SerializeOptions(SerializedScriptValue::kNotForStorage), exception_state);
  if (exception_state.HadException())
    return;

  impl->setSerializedScriptValueAttribute(std::move(cpp_value));
}

static void SerializedScriptValueMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info) {
  TestObject* impl = V8TestObject::ToImpl(info.Holder());

  V8SetReturnValue(info, V8Deserialize(info.GetIsolate(), impl->serializedScriptValueMethod().get()));
}

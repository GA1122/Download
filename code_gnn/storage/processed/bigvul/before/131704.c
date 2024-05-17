static void serializedScriptValueAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(RefPtr<SerializedScriptValue>, cppValue, SerializedScriptValue::create(jsValue, info.GetIsolate()));
    imp->setSerializedScriptValueAttribute(WTF::getPtr(cppValue));
}

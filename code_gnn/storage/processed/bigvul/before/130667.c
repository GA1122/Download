static void callbackFunctionAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, cppValue, ScriptValue(jsValue, info.GetIsolate()));
    imp->setCallbackFunctionAttribute(cppValue);
}

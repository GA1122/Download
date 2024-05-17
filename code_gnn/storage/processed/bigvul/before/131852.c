static void voidCallbackFunctionAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, cppValue, ScriptValue(jsValue, info.GetIsolate()));
    imp->setVoidCallbackFunctionAttribute(cppValue);
}

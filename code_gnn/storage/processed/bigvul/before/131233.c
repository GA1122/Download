static void booleanAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(bool, cppValue, jsValue->BooleanValue());
    imp->setBooleanAttribute(cppValue);
}
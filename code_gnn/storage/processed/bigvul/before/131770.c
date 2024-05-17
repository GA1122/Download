static void testEnumAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, cppValue, jsValue);
    String string = cppValue;
    if (!(string == "" || string == "EnumValue1" || string == "EnumValue2" || string == "EnumValue3"))
        return;
    imp->setTestEnumAttribute(cppValue);
}
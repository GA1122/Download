static void TestObjectReplaceableAttributeSetterCallback(v8::Local<v8::String> name, v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectV8Internal::TestObjectReplaceableAttributeSetter(name, jsValue, info);
}

static void nullableTestInterfaceAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterface*, cppValue, V8TestInterface::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setNullableTestInterfaceAttribute(WTF::getPtr(cppValue));
}
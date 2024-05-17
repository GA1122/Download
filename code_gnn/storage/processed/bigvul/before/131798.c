static void testObjectPythonAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestObjectPython*, cppValue, V8TestObjectPython::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setTestObjectPythonAttribute(WTF::getPtr(cppValue));
}

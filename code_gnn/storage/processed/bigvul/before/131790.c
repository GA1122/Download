static void testInterfacePythonAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfacePythonImplementation*, cppValue, V8TestInterfacePython::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setTestInterfacePythonAttribute(WTF::getPtr(cppValue));
}

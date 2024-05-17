static void testInterfaceWillBeGarbageCollectedAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(TestInterfaceWillBeGarbageCollected*, cppValue, V8TestInterfaceWillBeGarbageCollected::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setTestInterfaceWillBeGarbageCollectedAttribute(WTF::getPtr(cppValue));
}

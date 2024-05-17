static void testInterfaceEmptyArrayAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<RefPtr<TestInterfaceEmpty> >, cppValue, (toRefPtrNativeArray<TestInterfaceEmpty, V8TestInterfaceEmpty>(jsValue, 0, info.GetIsolate())));
    imp->setTestInterfaceEmptyArrayAttribute(cppValue);
}

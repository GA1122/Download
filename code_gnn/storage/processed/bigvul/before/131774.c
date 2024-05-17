static void testInterfaceEmptyArrayAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValue(info, v8Array(imp->testInterfaceEmptyArrayAttribute(), info.GetIsolate()));
}

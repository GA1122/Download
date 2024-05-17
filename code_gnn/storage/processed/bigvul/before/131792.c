static void testInterfaceWillBeGarbageCollectedAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    bool isNull = false;
    RefPtrWillBeRawPtr<TestInterfaceWillBeGarbageCollected> jsValue = imp->testInterfaceWillBeGarbageCollectedAttribute(isNull);
    if (isNull) {
        v8SetReturnValueNull(info);
        return;
    }
    v8SetReturnValueFast(info, WTF::getPtr(jsValue.release()), imp);
}

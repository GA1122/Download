static void nullableTestInterfaceAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    bool isNull = false;
    RefPtr<TestInterface> jsValue = imp->nullableTestInterfaceAttribute(isNull);
    if (isNull) {
        v8SetReturnValueNull(info);
        return;
    }
    v8SetReturnValueFast(info, WTF::getPtr(jsValue.release()), imp);
}

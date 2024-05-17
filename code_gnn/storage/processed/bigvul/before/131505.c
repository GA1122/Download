static void nullableLongAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    bool isNull = false;
    int jsValue = imp->nullableLongAttribute(isNull);
    if (isNull) {
        v8SetReturnValueNull(info);
        return;
    }
    v8SetReturnValueInt(info, jsValue);
}
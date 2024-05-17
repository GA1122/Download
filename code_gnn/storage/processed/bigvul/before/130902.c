static void nullableLongSettableAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    bool isNull = false;
    int jsValue = imp->nullableLongSettableAttribute(isNull);
    if (isNull) {
        v8SetReturnValueNull(info);
        return;
    }
    v8SetReturnValueInt(info, jsValue);
}

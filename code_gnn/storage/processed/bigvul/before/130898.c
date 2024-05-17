static void nullableDoubleAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    bool isNull = false;
    double jsValue = imp->nullableDoubleAttribute(isNull);
    if (isNull) {
        v8SetReturnValueNull(info);
        return;
    }
    v8SetReturnValue(info, jsValue);
}

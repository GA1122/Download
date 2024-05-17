static void enforcedRangeUnsignedShortAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueUnsigned(info, imp->enforcedRangeUnsignedShortAttr());
}

static void enforcedRangeLongLongAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValue(info, static_cast<double>(imp->enforcedRangeLongLongAttr()));
}

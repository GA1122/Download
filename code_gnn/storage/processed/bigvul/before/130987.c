static void reflectedCustomIntegralAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueInt(info, imp->getIntegralAttribute(HTMLNames::customContentIntegralAttrAttr));
}
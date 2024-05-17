static void reflectedUnsignedIntegralAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueUnsigned(info, std::max(0, imp->getIntegralAttribute(HTMLNames::reflectedunsignedintegralattrAttr)));
}

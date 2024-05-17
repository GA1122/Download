static void reflectedBooleanAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValueBool(info, imp->fastHasAttribute(HTMLNames::reflectedbooleanattrAttr));
}

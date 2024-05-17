static void deprecatedStaticReadOnlyAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8SetReturnValueInt(info, TestObject::deprecatedStaticReadOnlyAttr());
}

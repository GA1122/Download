static void staticStringAttrAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8SetReturnValueString(info, TestObject::staticStringAttr(), info.GetIsolate());
}

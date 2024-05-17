static void anotherStringAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueString(info, imp->fastGetAttribute(HTMLNames::ReflectUrlAttributeAsAStringAttr), info.GetIsolate());
}

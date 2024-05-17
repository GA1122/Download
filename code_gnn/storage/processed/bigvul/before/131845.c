static void urlStringAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueString(info, imp->getURLAttribute(HTMLNames::urlstringattributeAttr), info.GetIsolate());
}

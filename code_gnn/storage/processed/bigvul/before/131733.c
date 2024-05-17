static void staticStringAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8SetReturnValueString(info, TestObjectPython::staticStringAttribute(), info.GetIsolate());
}

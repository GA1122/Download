static void treatReturnedNullStringAsNullStringAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueStringOrNull(info, imp->treatReturnedNullStringAsNullStringAttribute(), info.GetIsolate());
}

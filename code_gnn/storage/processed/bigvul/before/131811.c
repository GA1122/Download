static void treatReturnedNullStringAsUndefinedStringAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    v8SetReturnValueStringOrUndefined(info, imp->treatReturnedNullStringAsUndefinedStringAttribute(), info.GetIsolate());
}

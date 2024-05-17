static void unforgeableLongAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::Object> holder = V8TestObjectPython::findInstanceInPrototypeChain(info.This(), info.GetIsolate());
    if (holder.IsEmpty())
        return;
    TestObjectPython* imp = V8TestObjectPython::toNative(holder);
    v8SetReturnValueInt(info, imp->unforgeableLongAttribute());
}

static void floatArrayAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    v8SetReturnValue(info, v8Array(imp->floatArray(), info.GetIsolate()));
}
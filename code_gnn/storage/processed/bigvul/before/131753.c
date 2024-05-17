static void stringArrayAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<String>, cppValue, toNativeArray<String>(jsValue, 0, info.GetIsolate()));
    imp->setStringArrayAttribute(cppValue);
}

static void doubleArrayAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<double>, cppValue, toNativeArray<double>(jsValue, 0, info.GetIsolate()));
    imp->setDoubleArray(cppValue);
}

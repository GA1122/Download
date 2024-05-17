static void messagePortArrayAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Vector<RefPtr<MessagePort> >, cppValue, (toRefPtrNativeArray<MessagePort, V8MessagePort>(jsValue, 0, info.GetIsolate())));
    imp->setMessagePortArray(cppValue);
}

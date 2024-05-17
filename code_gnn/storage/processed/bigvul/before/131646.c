static void readonlyTestInterfaceEmptyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    RefPtr<TestInterfaceEmpty> result(imp->readonlyTestInterfaceEmptyAttribute());
    if (result && DOMDataStore::setReturnValueFromWrapper<V8TestInterfaceEmpty>(info.GetReturnValue(), result.get()))
        return;
    v8::Handle<v8::Value> wrapper = toV8(result.get(), info.Holder(), info.GetIsolate());
    if (!wrapper.IsEmpty()) {
        V8HiddenValue::setHiddenValue(info.GetIsolate(), info.Holder(), v8AtomicString(info.GetIsolate(), "readonlyTestInterfaceEmptyAttribute"), wrapper);
        v8SetReturnValue(info, wrapper);
    }
}

static void perWorldReadOnlyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    RefPtr<TestObject> result(imp->perWorldReadOnlyAttribute());
    if (result && DOMDataStore::setReturnValueFromWrapper<V8TestObject>(info.GetReturnValue(), result.get()))
        return;
    v8::Handle<v8::Value> wrapper = toV8(result.get(), info.Holder(), info.GetIsolate());
    if (!wrapper.IsEmpty()) {
        V8HiddenValue::setHiddenValue(info.GetIsolate(), info.Holder(), v8AtomicString(info.GetIsolate(), "perWorldReadOnlyAttribute"), wrapper);
        v8SetReturnValue(info, wrapper);
    }
}

static void indexedPropertyGetter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    RefPtr<Node> result = imp->anonymousIndexedGetter(index);
    if (!result)
        return;
    v8SetReturnValueFast(info, WTF::getPtr(result.release()), imp);
}

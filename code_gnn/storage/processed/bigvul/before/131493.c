static void nodeFilterAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(RefPtr<NodeFilter>, cppValue, toNodeFilter(jsValue, info.GetIsolate()));
    imp->setNodeFilterAttribute(WTF::getPtr(cppValue));
}

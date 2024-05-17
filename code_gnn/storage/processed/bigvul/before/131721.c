static void shadowRootAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ShadowRoot*, cppValue, V8ShadowRoot::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setShadowRootAttribute(WTF::getPtr(cppValue));
}

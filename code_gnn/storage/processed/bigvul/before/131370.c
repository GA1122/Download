static void elementAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Element*, cppValue, V8Element::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setElementAttribute(WTF::getPtr(cppValue));
}
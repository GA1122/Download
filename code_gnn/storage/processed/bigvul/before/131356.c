static void documentFragmentAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(DocumentFragment*, cppValue, V8DocumentFragment::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setDocumentFragmentAttribute(WTF::getPtr(cppValue));
}

static void documentAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Document*, cppValue, V8Document::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setDocumentAttribute(WTF::getPtr(cppValue));
}

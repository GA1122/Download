static void documentTypeAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(DocumentType*, cppValue, V8DocumentType::toNativeWithTypeCheck(info.GetIsolate(), jsValue));
    imp->setDocumentTypeAttribute(WTF::getPtr(cppValue));
}

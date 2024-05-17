static void arrayBufferAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ArrayBuffer*, cppValue, jsValue->IsArrayBuffer() ? V8ArrayBuffer::toNative(v8::Handle<v8::ArrayBuffer>::Cast(jsValue)) : 0);
    imp->setArrayBufferAttribute(WTF::getPtr(cppValue));
}

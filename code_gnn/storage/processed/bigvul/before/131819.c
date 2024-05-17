static void uint8ArrayAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Uint8Array*, cppValue, jsValue->IsUint8Array() ? V8Uint8Array::toNative(v8::Handle<v8::Uint8Array>::Cast(jsValue)) : 0);
    imp->setUint8ArrayAttribute(WTF::getPtr(cppValue));
}

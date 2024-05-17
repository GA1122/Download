static void typedArrayAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(Float32Array*, cppValue, jsValue->IsFloat32Array() ? V8Float32Array::toNative(v8::Handle<v8::Float32Array>::Cast(jsValue)) : 0);
    imp->setTypedArrayAttr(WTF::getPtr(cppValue));
}

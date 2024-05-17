static void float32ArrayAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(Float32Array*, cppValue, jsValue->IsFloat32Array() ? V8Float32Array::toNative(v8::Handle<v8::Float32Array>::Cast(jsValue)) : 0);
    imp->setFloat32ArrayAttribute(WTF::getPtr(cppValue));
}

static void strictFloatAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(float, cppValue, static_cast<float>(jsValue->NumberValue()));
    imp->setStrictFloat(cppValue);
}

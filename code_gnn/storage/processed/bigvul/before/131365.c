static void doubleAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(double, cppValue, static_cast<double>(jsValue->NumberValue()));
    imp->setDoubleAttribute(cppValue);
}

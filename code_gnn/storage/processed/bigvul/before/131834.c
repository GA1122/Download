static void unsignedLongLongAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "unsignedLongLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned long long, cppValue, toUInt64(jsValue, exceptionState), exceptionState);
    imp->setUnsignedLongLongAttribute(cppValue);
}

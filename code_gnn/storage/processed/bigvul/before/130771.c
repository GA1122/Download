static void enforcedRangeUnsignedLongLongAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "enforcedRangeUnsignedLongLongAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned long long, cppValue, toUInt64(jsValue, EnforceRange, exceptionState), exceptionState);
    imp->setEnforcedRangeUnsignedLongLongAttr(cppValue);
}

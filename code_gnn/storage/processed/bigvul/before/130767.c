static void enforcedRangeUnsignedLongAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "enforcedRangeUnsignedLongAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned, cppValue, toUInt32(jsValue, EnforceRange, exceptionState), exceptionState);
    imp->setEnforcedRangeUnsignedLongAttr(cppValue);
}

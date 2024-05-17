static void enforcedRangeUnsignedShortAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "enforcedRangeUnsignedShortAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned, cppValue, toUInt16(jsValue, EnforceRange, exceptionState), exceptionState);
    imp->setEnforcedRangeUnsignedShortAttr(cppValue);
}

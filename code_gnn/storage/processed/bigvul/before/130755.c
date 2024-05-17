static void enforcedRangeLongLongAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "enforcedRangeLongLongAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(long long, cppValue, toInt64(jsValue, EnforceRange, exceptionState), exceptionState);
    imp->setEnforcedRangeLongLongAttr(cppValue);
}

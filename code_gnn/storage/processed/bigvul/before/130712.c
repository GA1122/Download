static void deprecatedAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "deprecatedAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    imp->setDeprecatedAttr(cppValue);
}
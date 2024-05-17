static void deprecatedStaticAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "deprecatedStaticAttr", "TestObject", info.Holder(), info.GetIsolate());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    TestObject::setDeprecatedStaticAttr(cppValue);
}

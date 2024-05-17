static void staticLongAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "staticLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    TestObjectPython::setStaticLongAttribute(cppValue);
}

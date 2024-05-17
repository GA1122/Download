static void conditionalOrLongAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "conditionalOrLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    imp->setConditionalOrLongAttribute(cppValue);
}

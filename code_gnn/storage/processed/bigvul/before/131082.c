static void stringAttrWithSetterExceptionAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "stringAttrWithSetterException", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, cppValue, jsValue);
    imp->setStringAttrWithSetterException(cppValue, exceptionState);
    exceptionState.throwIfNeeded();
}

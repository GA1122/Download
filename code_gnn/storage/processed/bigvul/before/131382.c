static void exposeJSAccessorsLongAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "exposeJSAccessorsLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    imp->setExposeJSAccessorsLongAttribute(cppValue);
}

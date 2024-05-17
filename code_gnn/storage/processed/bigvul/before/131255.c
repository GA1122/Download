static void callWithExecutionContextAnyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ExecutionContext* scriptContext = currentExecutionContext(info.GetIsolate());
    v8SetReturnValue(info, imp->callWithExecutionContextAnyAttribute(scriptContext).v8Value());
}

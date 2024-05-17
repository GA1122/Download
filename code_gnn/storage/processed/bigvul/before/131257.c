static void callWithExecutionContextVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    ExecutionContext* scriptContext = currentExecutionContext(info.GetIsolate());
    imp->callWithExecutionContextVoidMethod(scriptContext);
}

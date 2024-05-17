static void withExecutionContextMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    ExecutionContext* scriptContext = currentExecutionContext(info.GetIsolate());
    imp->withExecutionContext(scriptContext);
}

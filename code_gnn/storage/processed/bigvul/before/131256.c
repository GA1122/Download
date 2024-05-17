static void callWithExecutionContextAnyAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_VOID(ScriptValue, cppValue, ScriptValue(jsValue, info.GetIsolate()));
    ExecutionContext* scriptContext = currentExecutionContext(info.GetIsolate());
    imp->setCallWithExecutionContextAnyAttribute(scriptContext, cppValue);
}

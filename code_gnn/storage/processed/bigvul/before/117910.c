static v8::Handle<v8::Value> withScriptExecutionContextCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptExecutionContext");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    ScriptExecutionContext* scriptContext = getScriptExecutionContext();
    if (!scriptContext)
        return v8::Undefined();
    imp->withScriptExecutionContext(scriptContext);
    return v8::Handle<v8::Value>();
}

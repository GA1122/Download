static v8::Handle<v8::Value> withScriptArgumentsAndCallStackCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.withScriptArgumentsAndCallStack");
    TestObj* imp = V8TestObj::toNative(args.Holder());
    RefPtr<ScriptArguments> scriptArguments(createScriptArguments(args, 0));
    RefPtr<ScriptCallStack> callStack(createScriptCallStackForInspector());
    if (!callStack)
        return v8::Undefined();
    imp->withScriptArgumentsAndCallStack(scriptArguments, callStack);
    return v8::Handle<v8::Value>();
}

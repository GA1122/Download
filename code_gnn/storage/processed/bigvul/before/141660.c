void V8Console::debugFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    v8::Local<v8::Function> function;
    if (!helper.firstArgAsFunction().ToLocal(&function))
        return;
    setFunctionBreakpoint(helper, function, V8DebuggerAgentImpl::DebugCommandBreakpointSource, String16(), true);
}

void V8Console::unmonitorFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    v8::Local<v8::Function> function;
    if (!helper.firstArgAsFunction().ToLocal(&function))
        return;
    setFunctionBreakpoint(helper, function, V8DebuggerAgentImpl::MonitorCommandBreakpointSource, String16(), false);
}

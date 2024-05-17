static void setFunctionBreakpoint(ConsoleHelper& helper, v8::Local<v8::Function> function, V8DebuggerAgentImpl::BreakpointSource source, const String16& condition, bool enable)
{
    V8DebuggerAgentImpl* debuggerAgent = helper.debuggerAgent();
    if (!debuggerAgent)
        return;
    String16 scriptId = String16::fromInteger(function->ScriptId());
    int lineNumber = function->GetScriptLineNumber();
    int columnNumber = function->GetScriptColumnNumber();
    if (lineNumber == v8::Function::kLineOffsetNotFound || columnNumber == v8::Function::kLineOffsetNotFound)
        return;
    if (enable)
        debuggerAgent->setBreakpointAt(scriptId, lineNumber, columnNumber, source, condition);
    else
        debuggerAgent->removeBreakpointAt(scriptId, lineNumber, columnNumber, source);
}

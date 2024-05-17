void V8Console::profileEndCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    if (V8ProfilerAgentImpl* profilerAgent = helper.profilerAgent())
        profilerAgent->consoleProfileEnd(helper.firstArgToString(String16()));
}

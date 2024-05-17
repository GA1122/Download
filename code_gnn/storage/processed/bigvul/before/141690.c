void V8Console::profileCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    if (V8ProfilerAgentImpl* profilerAgent = helper.profilerAgent())
        profilerAgent->consoleProfile(helper.firstArgToString(String16()));
}

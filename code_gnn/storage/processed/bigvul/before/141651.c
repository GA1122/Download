void V8Console::assertCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    if (helper.firstArgToBoolean(false))
        return;

    std::vector<v8::Local<v8::Value>> arguments;
    for (int i = 1; i < info.Length(); ++i)
        arguments.push_back(info[i]);
    if (info.Length() < 2)
        arguments.push_back(toV8String(info.GetIsolate(), String16("console.assert")));
    helper.reportCall(ConsoleAPIType::kAssert, arguments);

    if (V8DebuggerAgentImpl* debuggerAgent = helper.debuggerAgent())
        debuggerAgent->breakProgramOnException(protocol::Debugger::Paused::ReasonEnum::Assert, nullptr);
}

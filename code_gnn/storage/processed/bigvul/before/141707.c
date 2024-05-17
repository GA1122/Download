void V8Console::timeStampCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    if (V8InspectorClient* client = helper.ensureDebuggerClient())
        client->consoleTimeStamp(helper.firstArgToString(String16()));
}

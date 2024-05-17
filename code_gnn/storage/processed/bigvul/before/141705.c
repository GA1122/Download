static void timeEndFunction(const v8::FunctionCallbackInfo<v8::Value>& info, bool timelinePrefix)
{
    ConsoleHelper helper(info);
    if (V8InspectorClient* client = helper.ensureDebuggerClient()) {
        String16 protocolTitle = helper.firstArgToString("default");
        if (timelinePrefix)
            protocolTitle = "Timeline '" + protocolTitle + "'";
        client->consoleTimeEnd(protocolTitle);

        v8::Local<v8::Map> timeMap;
        if (!helper.privateMap("V8Console#timeMap").ToLocal(&timeMap))
            return;
        double elapsed = client->currentTimeMS() - helper.getDoubleFromMap(timeMap, protocolTitle, 0.0);
        String16 message = protocolTitle + ": " + String16::fromDoublePrecision3(elapsed) + "ms";
        helper.reportCallWithArgument(ConsoleAPIType::kTimeEnd, message);
    }
}

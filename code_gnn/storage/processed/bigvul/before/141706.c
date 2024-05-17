static void timeFunction(const v8::FunctionCallbackInfo<v8::Value>& info, bool timelinePrefix)
{
    ConsoleHelper helper(info);
    if (V8InspectorClient* client = helper.ensureDebuggerClient()) {
        String16 protocolTitle = helper.firstArgToString("default");
        if (timelinePrefix)
            protocolTitle = "Timeline '" + protocolTitle + "'";
        client->consoleTime(protocolTitle);

        v8::Local<v8::Map> timeMap;
        if (!helper.privateMap("V8Console#timeMap").ToLocal(&timeMap))
            return;
        helper.setDoubleOnMap(timeMap, protocolTitle, client->currentTimeMS());
    }
}

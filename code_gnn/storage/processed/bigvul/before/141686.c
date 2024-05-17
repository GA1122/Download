void V8Console::memoryGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (V8InspectorClient* client = ConsoleHelper(info).ensureDebuggerClient()) {
        v8::Local<v8::Value> memoryValue;
        if (!client->memoryInfo(info.GetIsolate(), info.GetIsolate()->GetCurrentContext()).ToLocal(&memoryValue))
            return;
        info.GetReturnValue().Set(memoryValue);
    }
}

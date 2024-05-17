static void perWorldBindingsVoidMethodTestInterfaceEmptyArgMethodCallbackForMainWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    TestObjectPythonV8Internal::perWorldBindingsVoidMethodTestInterfaceEmptyArgMethodForMainWorld(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

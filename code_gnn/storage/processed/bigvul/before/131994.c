static void voidMethodVariadicTestInterfaceWillBeGarbageCollectedArgMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    TestObjectPythonV8Internal::voidMethodVariadicTestInterfaceWillBeGarbageCollectedArgMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
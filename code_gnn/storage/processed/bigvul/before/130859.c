static void methodWithEnforceRangeInt8MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    TestObjectV8Internal::methodWithEnforceRangeInt8Method(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

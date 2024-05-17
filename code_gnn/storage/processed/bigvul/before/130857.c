static void methodWithEnforceRangeInt64MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    TestObjectV8Internal::methodWithEnforceRangeInt64Method(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

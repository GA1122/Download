static void conditionalMethod2MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    TestObjectV8Internal::conditionalMethod2Method(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
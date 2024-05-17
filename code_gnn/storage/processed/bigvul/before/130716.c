static void deprecatedMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    UseCounter::countDeprecation(callingExecutionContext(info.GetIsolate()), UseCounter::Method);
    TestObjectV8Internal::deprecatedMethodMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

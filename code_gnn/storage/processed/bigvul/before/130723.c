static void deprecatedStaticMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    UseCounter::countDeprecation(callingExecutionContext(info.GetIsolate()), UseCounter::StaticMethod);
    TestObjectV8Internal::deprecatedStaticMethodMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

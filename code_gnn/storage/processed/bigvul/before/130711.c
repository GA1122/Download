static void deprecatedAttrAttributeGetterCallback(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMGetter");
    UseCounter::countDeprecation(callingExecutionContext(info.GetIsolate()), UseCounter::Attribute);
    TestObjectV8Internal::deprecatedAttrAttributeGetter(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
static void activityLoggedInIsolatedWorldsAttrGetterAttributeGetterCallback(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMGetter");
    V8PerContextData* contextData = V8PerContextData::from(info.GetIsolate()->GetCurrentContext());
    if (contextData && contextData->activityLogger())
        contextData->activityLogger()->log("TestObject.activityLoggedInIsolatedWorldsAttrGetter", 0, 0, "Getter");
    TestObjectV8Internal::activityLoggedInIsolatedWorldsAttrGetterAttributeGetter(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

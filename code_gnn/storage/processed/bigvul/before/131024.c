static void reflectedTreatNullAsNullStringTreatUndefinedAsNullStringCustomURLAttrAttributeGetterCallback(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMGetter");
    TestObjectV8Internal::reflectedTreatNullAsNullStringTreatUndefinedAsNullStringCustomURLAttrAttributeGetter(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
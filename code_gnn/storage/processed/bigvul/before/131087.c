static void testObjAttrAttributeSetterCallback(v8::Local<v8::String>, v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMSetter");
    UseCounter::count(callingExecutionContext(info.GetIsolate()), UseCounter::TestFeature);
    TestObjectV8Internal::testObjAttrAttributeSetter(jsValue, info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
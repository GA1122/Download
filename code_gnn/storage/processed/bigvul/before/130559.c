static void TestSubObjMeasuredConstructorGetterCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMGetter");
    UseCounter::count(callingExecutionContext(info.GetIsolate()), UseCounter::TestFeature);
    TestObjectV8Internal::TestObjectConstructorGetter(property, info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

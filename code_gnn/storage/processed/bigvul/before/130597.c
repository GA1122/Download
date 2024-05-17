static void activityLoggedAttrSetter2AttributeSetterCallback(v8::Local<v8::String>, v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMSetter");
    V8PerContextData* contextData = V8PerContextData::from(info.GetIsolate()->GetCurrentContext());
    if (contextData && contextData->activityLogger()) {
        v8::Handle<v8::Value> loggerArg[] = { jsValue };
        contextData->activityLogger()->log("TestObject.activityLoggedAttrSetter2", 1, &loggerArg[0], "Setter");
    }
    TestObjectV8Internal::activityLoggedAttrSetter2AttributeSetter(jsValue, info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
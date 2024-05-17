static void activityLoggedMethod2MethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    V8PerContextData* contextData = V8PerContextData::from(info.GetIsolate()->GetCurrentContext());
    if (contextData && contextData->activityLogger()) {
        Vector<v8::Handle<v8::Value> > loggerArgs = toNativeArguments<v8::Handle<v8::Value> >(info, 0);
        contextData->activityLogger()->log("TestObject.activityLoggedMethod2", info.Length(), loggerArgs.data(), "Method");
    }
    TestObjectV8Internal::activityLoggedMethod2Method(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

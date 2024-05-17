static void measureAsVoidMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    UseCounter::count(callingExecutionContext(info.GetIsolate()), UseCounter::TestFeature);
    TestObjectPythonV8Internal::measureAsVoidMethodMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

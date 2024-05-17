static void customMethodMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    UseCounter::count(callingExecutionContext(info.GetIsolate()), UseCounter::CustomTestFeature);
    V8TestObject::customMethodMethodCustom(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

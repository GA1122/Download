static void reflectedTreatNullAsNullStringURLAttrAttributeSetterCallback(v8::Local<v8::String>, v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMSetter");
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    TestObjectV8Internal::reflectedTreatNullAsNullStringURLAttrAttributeSetter(jsValue, info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}
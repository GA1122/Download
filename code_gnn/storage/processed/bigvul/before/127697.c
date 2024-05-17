static void reflectStringAttributeAttributeSetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Value> v8Value = info[0];
    V0CustomElementProcessingStack::CallbackDeliveryScope deliveryScope;
    TestInterfaceNodeV8Internal::reflectStringAttributeAttributeSetter(v8Value, info);
}

static void eventHandlerAttributeAttributeSetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Value> v8Value = info[0];
    TestInterfaceNodeV8Internal::eventHandlerAttributeAttributeSetter(v8Value, info);
}

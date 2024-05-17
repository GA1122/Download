static void customElementCallbacksVoidMethodMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->customElementCallbacksVoidMethod();
}

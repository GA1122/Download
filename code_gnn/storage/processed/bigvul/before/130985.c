static void reflectedCustomBooleanAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_VOID(bool, cppValue, jsValue->BooleanValue());
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->setBooleanAttribute(HTMLNames::customContentBooleanAttrAttr, cppValue);
}

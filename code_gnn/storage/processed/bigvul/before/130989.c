static void reflectedCustomIntegralAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "reflectedCustomIntegralAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->setIntegralAttribute(HTMLNames::customContentIntegralAttrAttr, cppValue);
}

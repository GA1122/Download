static void reflectedIntegralAttrAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "reflectedIntegralAttr", "TestObject", info.Holder(), info.GetIsolate());
    TestObject* imp = V8TestObject::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(int, cppValue, toInt32(jsValue, exceptionState), exceptionState);
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->setIntegralAttribute(HTMLNames::reflectedintegralattrAttr, cppValue);
}

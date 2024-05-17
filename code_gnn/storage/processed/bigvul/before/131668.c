static void reflectUnsignedLongAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "reflectUnsignedLongAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned, cppValue, toUInt32(jsValue, exceptionState), exceptionState);
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->setUnsignedIntegralAttribute(HTMLNames::reflectunsignedlongattributeAttr, cppValue);
}

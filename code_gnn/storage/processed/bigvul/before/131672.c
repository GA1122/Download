static void reflectUnsignedShortAttributeAttributeSetter(v8::Local<v8::Value> jsValue, const v8::PropertyCallbackInfo<void>& info)
{
    ExceptionState exceptionState(ExceptionState::SetterContext, "reflectUnsignedShortAttribute", "TestObjectPython", info.Holder(), info.GetIsolate());
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    V8TRYCATCH_EXCEPTION_VOID(unsigned, cppValue, toUInt16(jsValue, exceptionState), exceptionState);
    CustomElementCallbackDispatcher::CallbackDeliveryScope deliveryScope;
    imp->setAttribute(HTMLNames::reflectunsignedshortattributeAttr, cppValue);
}

static void corsSettingAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::corssettingattributeAttr);
    if (resultValue.isNull()) {
        ;
    } else if (resultValue.isEmpty()) {
        resultValue = "anonymous";
    } else if (equalIgnoringCase(resultValue, "anonymous")) {
        resultValue = "anonymous";
    } else if (equalIgnoringCase(resultValue, "use-credentials")) {
        resultValue = "use-credentials";
    } else {
        resultValue = "anonymous";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

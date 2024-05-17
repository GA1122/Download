static void limitedToOnlyAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::limitedtoonlyattributeAttr);
    if (resultValue.isEmpty()) {
        ;
    } else if (equalIgnoringCase(resultValue, "Per")) {
        resultValue = "Per";
    } else if (equalIgnoringCase(resultValue, "Paal")) {
        resultValue = "Paal";
    } else if (equalIgnoringCase(resultValue, "Espen")) {
        resultValue = "Espen";
    } else {
        resultValue = "";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

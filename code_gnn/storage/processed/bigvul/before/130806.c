static void limitedWithEmptyMissingInvalidAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::limitedwithemptymissinginvalidattributeAttr);
    if (resultValue.isNull()) {
        resultValue = "missing";
    } else if (resultValue.isEmpty()) {
        resultValue = "empty";
    } else if (equalIgnoringCase(resultValue, "empty")) {
        resultValue = "empty";
    } else if (equalIgnoringCase(resultValue, "missing")) {
        resultValue = "missing";
    } else if (equalIgnoringCase(resultValue, "invalid")) {
        resultValue = "invalid";
    } else if (equalIgnoringCase(resultValue, "a-normal")) {
        resultValue = "a-normal";
    } else {
        resultValue = "invalid";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

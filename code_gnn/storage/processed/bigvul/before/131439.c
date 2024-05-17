static void limitedWithMissingDefaultAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::limitedwithmissingdefaultattributeAttr);
    if (resultValue.isEmpty()) {
        resultValue = "rsa";
    } else if (equalIgnoringCase(resultValue, "rsa")) {
        resultValue = "rsa";
    } else if (equalIgnoringCase(resultValue, "dsa")) {
        resultValue = "dsa";
    } else {
        resultValue = "";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

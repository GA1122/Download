static void limitedToOnlyOneAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObjectPython* imp = V8TestObjectPython::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::limitedtoonlyoneattributeAttr);
    if (resultValue.isEmpty()) {
        ;
    } else if (equalIgnoringCase(resultValue, "unique")) {
        resultValue = "unique";
    } else {
        resultValue = "";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

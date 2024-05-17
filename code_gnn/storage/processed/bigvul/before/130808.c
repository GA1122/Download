static void limitedWithInvalidAndMissingDefaultAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::limitedwithinvalidandmissingdefaultattributeAttr);
    if (resultValue.isEmpty()) {
        resultValue = "left";
    } else if (equalIgnoringCase(resultValue, "left")) {
        resultValue = "left";
    } else if (equalIgnoringCase(resultValue, "right")) {
        resultValue = "right";
    } else {
        resultValue = "left";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

static void limitedToOnlyOtherAttributeAttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TestObject* imp = V8TestObject::toNative(info.Holder());
    String resultValue = imp->fastGetAttribute(HTMLNames::OtherAttr);
    if (resultValue.isEmpty()) {
        ;
    } else if (equalIgnoringCase(resultValue, "Value1")) {
        resultValue = "Value1";
    } else if (equalIgnoringCase(resultValue, "Value2")) {
        resultValue = "Value2";
    } else {
        resultValue = "";
    }
    v8SetReturnValueString(info, resultValue, info.GetIsolate());
}

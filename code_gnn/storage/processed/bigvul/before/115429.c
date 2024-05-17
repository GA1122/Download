static WTF::String dumpPath(JSGlobalContextRef context, JSObjectRef nodeValue)
{
    JSValueRef nodeNameValue = propertyValue(context, nodeValue, "nodeName");
    JSRetainPtr<JSStringRef> jsStringNodeName(Adopt, JSValueToStringCopy(context, nodeNameValue, 0));
    WKRetainPtr<WKStringRef> nodeName = toWK(jsStringNodeName);

    JSValueRef parentNode = propertyValue(context, nodeValue, "parentNode");

    StringBuilder stringBuilder;
    stringBuilder.append(toWTFString(nodeName));

    if (parentNode && JSValueIsObject(context, parentNode)) {
        stringBuilder.appendLiteral(" > ");
        stringBuilder.append(dumpPath(context, (JSObjectRef)parentNode));
    }

    return stringBuilder.toString();
}

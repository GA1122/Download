static bool hasDocumentElement(WKBundleFrameRef frame)
{
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContext(frame);
    JSObjectRef globalObject = JSContextGetGlobalObject(context);

    JSValueRef documentValue = JSObjectGetProperty(context, globalObject, toJS("document").get(), 0);
    if (!documentValue)
        return false;

    ASSERT(JSValueIsObject(context, documentValue));
    JSObjectRef document = JSValueToObject(context, documentValue, 0);

    JSValueRef documentElementValue = JSObjectGetProperty(context, document, toJS("documentElement").get(), 0);
    if (!documentElementValue)
        return false;

    return JSValueToBoolean(context, documentElementValue);
}

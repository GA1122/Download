static double numericWindowPropertyValue(WKBundleFrameRef frame, const char* propertyName)
{
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContext(frame);
    return propertyValueDouble(context, JSContextGetGlobalObject(context), propertyName);
}

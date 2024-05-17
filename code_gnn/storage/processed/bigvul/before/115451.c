static WTF::String rangeToStr(WKBundlePageRef page, WKBundleScriptWorldRef world, WKBundleRangeHandleRef rangeRef)
{
    if (!rangeRef)
        return "(null)";
 
    WKBundleFrameRef frame = WKBundlePageGetMainFrame(page);

    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContextForWorld(frame, world);
    JSValueRef rangeValue = WKBundleFrameGetJavaScriptWrapperForRangeForWorld(frame, rangeRef, world);
    ASSERT(JSValueIsObject(context, rangeValue));
    JSObjectRef rangeObject = (JSObjectRef)rangeValue;

    JSValueRef startNodeValue = propertyValue(context, rangeObject, "startContainer");
    ASSERT(JSValueIsObject(context, startNodeValue));
    JSObjectRef startNodeObject = (JSObjectRef)startNodeValue;

    JSValueRef endNodeValue = propertyValue(context, rangeObject, "endContainer");
    ASSERT(JSValueIsObject(context, endNodeValue));
    JSObjectRef endNodeObject = (JSObjectRef)endNodeValue;

    int startOffset = propertyValueInt(context, rangeObject, "startOffset");
    int endOffset = propertyValueInt(context, rangeObject, "endOffset");

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("range from ");
    stringBuilder.appendNumber(startOffset);
    stringBuilder.appendLiteral(" of ");
    stringBuilder.append(dumpPath(context, startNodeObject));
    stringBuilder.appendLiteral(" to ");
    stringBuilder.appendNumber(endOffset);
    stringBuilder.appendLiteral(" of ");
    stringBuilder.append(dumpPath(context, endNodeObject));
    return stringBuilder.toString();
}

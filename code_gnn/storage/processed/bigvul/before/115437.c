static WTF::String frameToStr(WKBundleFrameRef frame)
{
    WKRetainPtr<WKStringRef> name(AdoptWK, WKBundleFrameCopyName(frame));
    StringBuilder stringBuilder;
    if (WKBundleFrameIsMainFrame(frame)) {
        if (!WKStringIsEmpty(name.get())) {
            stringBuilder.appendLiteral("main frame \"");
            stringBuilder.append(toWTFString(name));
            stringBuilder.append('"');
        } else
            stringBuilder.appendLiteral("main frame");
    } else {
        if (!WKStringIsEmpty(name.get())) {
            stringBuilder.appendLiteral("frame \"");
            stringBuilder.append(toWTFString(name));
            stringBuilder.append('"');
        }
        else
            stringBuilder.appendLiteral("frame (anonymous)");
    }
    
    return stringBuilder.toString();
}

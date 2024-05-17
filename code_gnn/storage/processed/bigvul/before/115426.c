static void dumpFrameScrollPosition(WKBundleFrameRef frame, StringBuilder& stringBuilder, FrameNamePolicy shouldIncludeFrameName = ShouldNotIncludeFrameName)
{
    double x = numericWindowPropertyValue(frame, "pageXOffset");
    double y = numericWindowPropertyValue(frame, "pageYOffset");
    if (fabs(x) <= 0.00000001 && fabs(y) <= 0.00000001)
        return;

    if (shouldIncludeFrameName) {
        WKRetainPtr<WKStringRef> name(AdoptWK, WKBundleFrameCopyName(frame));
        stringBuilder.appendLiteral("frame '");
        stringBuilder.append(toWTFString(name));
        stringBuilder.appendLiteral("' ");
    }
    stringBuilder.appendLiteral("scrolled to ");
    stringBuilder.append(WTF::String::number(x));
    stringBuilder.append(',');
    stringBuilder.append(WTF::String::number(y));
    stringBuilder.append('\n');
}

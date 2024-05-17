static void dumpFrameDescriptionSuitableForTestResult(WKBundleFrameRef frame, StringBuilder& stringBuilder)
{
    WKRetainPtr<WKStringRef> name(AdoptWK, WKBundleFrameCopyName(frame));
    if (WKBundleFrameIsMainFrame(frame)) {
        if (WKStringIsEmpty(name.get())) {
            stringBuilder.appendLiteral("main frame");
            return;
        }

        stringBuilder.appendLiteral("main frame \"");
        stringBuilder.append(toWTFString(name));
        stringBuilder.append('"');
        return;
    }

    if (WKStringIsEmpty(name.get())) {
        stringBuilder.appendLiteral("frame (anonymous)");
        return;
    }

    stringBuilder.appendLiteral("frame \"");
    stringBuilder.append(toWTFString(name));
    stringBuilder.append('"');
}

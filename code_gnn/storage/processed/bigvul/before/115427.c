static void dumpFrameText(WKBundleFrameRef frame, StringBuilder& stringBuilder)
{
    if (!hasDocumentElement(frame))
        return;

    WKRetainPtr<WKStringRef> text(AdoptWK, WKBundleFrameCopyInnerText(frame));
    stringBuilder.append(toWTFString(text));
    stringBuilder.append('\n');
}

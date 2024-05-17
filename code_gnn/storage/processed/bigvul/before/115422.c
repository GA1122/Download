static void dumpDescendantFrameScrollPositions(WKBundleFrameRef frame, StringBuilder& stringBuilder)
{
    WKRetainPtr<WKArrayRef> childFrames(AdoptWK, WKBundleFrameCopyChildFrames(frame));
    size_t size = WKArrayGetSize(childFrames.get());
    for (size_t i = 0; i < size; ++i) {
        WKBundleFrameRef subframe = static_cast<WKBundleFrameRef>(WKArrayGetItemAtIndex(childFrames.get(), i));
        dumpFrameScrollPosition(subframe, stringBuilder, ShouldIncludeFrameName);
        dumpDescendantFrameScrollPositions(subframe, stringBuilder);
    }
}

void InjectedBundlePage::didFinishDocumentLoadForFrame(WKBundleFrameRef frame)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        dumpLoadEvent(frame, "didFinishDocumentLoadForFrame");

    unsigned pendingFrameUnloadEvents = WKBundleFrameGetPendingUnloadCount(frame);
    if (pendingFrameUnloadEvents) {
        StringBuilder stringBuilder;
        stringBuilder.append(frameToStr(frame));
        stringBuilder.appendLiteral(" - has ");
        stringBuilder.appendNumber(pendingFrameUnloadEvents);
        stringBuilder.appendLiteral(" onunload handler(s)\n");
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }
}

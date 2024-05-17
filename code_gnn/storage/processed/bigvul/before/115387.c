void InjectedBundlePage::didFailLoadWithErrorForFrame(WKBundleFrameRef frame, WKErrorRef)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        dumpLoadEvent(frame, "didFailLoadWithError");

    frameDidChangeLocation(frame);
}

void InjectedBundlePage::didStartProvisionalLoadForFrame(WKBundleFrameRef frame)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    platformDidStartProvisionalLoadForFrame(frame);

    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        dumpLoadEvent(frame, "didStartProvisionalLoadForFrame");

    if (!InjectedBundle::shared().topLoadingFrame())
        InjectedBundle::shared().setTopLoadingFrame(frame);

    if (InjectedBundle::shared().testRunner()->shouldStopProvisionalFrameLoads())
        dumpLoadEvent(frame, "stopping load in didStartProvisionalLoadForFrame callback");
}

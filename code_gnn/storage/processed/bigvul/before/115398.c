void InjectedBundlePage::didHandleOnloadEventsForFrame(WKBundleFrameRef frame)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        dumpLoadEvent(frame, "didHandleOnloadEventsForFrame");
}

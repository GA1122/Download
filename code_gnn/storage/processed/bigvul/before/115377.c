void InjectedBundlePage::didDetectXSSForFrame(WKBundleFrameRef frame)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        InjectedBundle::shared().outputText("didDetectXSS\n");
}

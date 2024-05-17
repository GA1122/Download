void InjectedBundlePage::didRunInsecureContentForFrame(WKBundleFrameRef frame)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        InjectedBundle::shared().outputText("didRunInsecureContent\n");
}

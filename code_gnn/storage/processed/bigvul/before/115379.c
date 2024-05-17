void InjectedBundlePage::didDisplayInsecureContentForFrame(WKBundleFrameRef frame)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFrameLoadCallbacks())
        InjectedBundle::shared().outputText("didDisplayInsecureContent\n");
}

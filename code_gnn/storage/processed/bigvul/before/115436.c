void InjectedBundlePage::frameDidChangeLocation(WKBundleFrameRef frame, bool shouldDump)
{
    if (frame != InjectedBundle::shared().topLoadingFrame())
        return;

    InjectedBundle::shared().setTopLoadingFrame(0);

    if (InjectedBundle::shared().testRunner()->waitToDump())
        return;

    if (InjectedBundle::shared().shouldProcessWorkQueue()) {
        InjectedBundle::shared().processWorkQueue();
        return;
    }

    if (shouldDump)
        InjectedBundle::shared().page()->dump();
    else
        InjectedBundle::shared().done();
}

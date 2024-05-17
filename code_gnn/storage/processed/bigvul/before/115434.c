void InjectedBundlePage::enterFullScreenForElement(WKBundlePageRef pageRef, WKBundleNodeHandleRef elementRef)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFullScreenCallbacks())
        InjectedBundle::shared().outputText("enterFullScreenForElement()\n");

    if (!InjectedBundle::shared().testRunner()->hasCustomFullScreenBehavior()) {
        WKBundlePageWillEnterFullScreen(pageRef);
        WKBundlePageDidEnterFullScreen(pageRef);
    }
}

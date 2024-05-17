void InjectedBundlePage::exitFullScreenForElement(WKBundlePageRef pageRef, WKBundleNodeHandleRef elementRef)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFullScreenCallbacks())
        InjectedBundle::shared().outputText("exitFullScreenForElement()\n");

    if (!InjectedBundle::shared().testRunner()->hasCustomFullScreenBehavior()) {
        WKBundlePageWillExitFullScreen(pageRef);
        WKBundlePageDidExitFullScreen(pageRef);
    }
}

void InjectedBundlePage::closeFullScreen(WKBundlePageRef pageRef)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFullScreenCallbacks())
        InjectedBundle::shared().outputText("closeFullScreen()\n");

    if (!InjectedBundle::shared().testRunner()->hasCustomFullScreenBehavior()) {
        WKBundlePageWillExitFullScreen(pageRef);
        WKBundlePageDidExitFullScreen(pageRef);
    }
}

void InjectedBundlePage::beganExitFullScreen(WKBundlePageRef, WKRect, WKRect)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpFullScreenCallbacks())
        InjectedBundle::shared().outputText("beganExitFullScreen()\n");
}

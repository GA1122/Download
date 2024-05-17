void InjectedBundlePage::didInitiateLoadForResource(WKBundlePageRef, WKBundleFrameRef, uint64_t identifier, WKURLRequestRef request, bool)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpResourceLoadCallbacks())
        return;

    WKRetainPtr<WKURLRef> url = adoptWK(WKURLRequestCopyURL(request));
    assignedUrlsCache.add(identifier, pathSuitableForTestResult(url.get()));
}

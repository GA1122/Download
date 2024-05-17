WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNavigationAction(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleNavigationActionRef navigationAction, WKURLRequestRef request, WKTypeRef* userData)
{
    if (!InjectedBundle::shared().isTestRunning())
        return WKBundlePagePolicyActionUse;

    if (!InjectedBundle::shared().testRunner()->isPolicyDelegateEnabled())
        return WKBundlePagePolicyActionUse;

    WKRetainPtr<WKURLRef> url = adoptWK(WKURLRequestCopyURL(request));
    WKRetainPtr<WKStringRef> urlScheme = adoptWK(WKURLCopyScheme(url.get()));

    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("Policy delegate: attempt to load ");
    if (isLocalFileScheme(urlScheme.get())) {
        WKRetainPtr<WKStringRef> filename = adoptWK(WKURLCopyLastPathComponent(url.get()));
        stringBuilder.append(toWTFString(filename));
    } else {
        WKRetainPtr<WKStringRef> urlString = adoptWK(WKURLCopyString(url.get()));
        stringBuilder.append(toWTFString(urlString));
    }
    stringBuilder.appendLiteral(" with navigation type \'");
    stringBuilder.append(toWTFString(NavigationTypeToString(WKBundleNavigationActionGetNavigationType(navigationAction))));
    stringBuilder.appendLiteral("\'");
    WKBundleHitTestResultRef hitTestResultRef = WKBundleNavigationActionCopyHitTestResult(navigationAction);
    if (hitTestResultRef) {
        stringBuilder.appendLiteral(" originating from ");
        stringBuilder.append(dumpPath(m_page, m_world.get(), WKBundleHitTestResultCopyNodeHandle(hitTestResultRef)));
    }

    stringBuilder.append('\n');
    InjectedBundle::shared().outputText(stringBuilder.toString());
    InjectedBundle::shared().testRunner()->notifyDone();

    if (InjectedBundle::shared().testRunner()->isPolicyDelegatePermissive())
        return WKBundlePagePolicyActionUse;
    return WKBundlePagePolicyActionPassThrough;
}

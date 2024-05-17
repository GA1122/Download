WKBundlePagePolicyAction InjectedBundlePage::decidePolicyForNewWindowAction(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleNavigationActionRef navigationAction, WKURLRequestRef request, WKStringRef frameName, WKTypeRef* userData, const void* clientInfo)
{
    return static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->decidePolicyForNewWindowAction(page, frame, navigationAction, request, frameName, userData);
}

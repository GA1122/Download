void InjectedBundlePage::didStartProvisionalLoadForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKTypeRef*, const void *clientInfo)
{
    static_cast<InjectedBundlePage*>(const_cast<void*>(clientInfo))->didStartProvisionalLoadForFrame(frame);
}
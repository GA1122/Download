Page* ChromeClientImpl::createWindow(Frame* frame, const FrameLoadRequest& r, const WindowFeatures& features,
    NavigationPolicy navigationPolicy, ShouldSendReferrer shouldSendReferrer)
{
    if (!m_webView->client())
        return 0;

    WebNavigationPolicy policy = static_cast<WebNavigationPolicy>(navigationPolicy);
    if (policy == WebNavigationPolicyIgnore)
        policy = getNavigationPolicy();

    DocumentFullscreen::webkitCancelFullScreen(frame->document());

    WebViewImpl* newView = toWebViewImpl(
        m_webView->client()->createView(WebFrameImpl::fromFrame(frame), WrappedResourceRequest(r.resourceRequest()), features, r.frameName(), policy, shouldSendReferrer == NeverSendReferrer));
    if (!newView)
        return 0;
    return newView->page();
}

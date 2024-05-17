PassOwnPtr<NavigatorContentUtilsClientImpl> NavigatorContentUtilsClientImpl::create(WebViewImpl* webView)
{
    return adoptPtr(new NavigatorContentUtilsClientImpl(webView));
}

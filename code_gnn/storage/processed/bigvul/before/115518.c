void TestController::ensureViewSupportsOptions(WKDictionaryRef options)
{
    if (m_mainWebView && !m_mainWebView->viewSupportsOptions(options)) {
        WKPageSetPageUIClient(m_mainWebView->page(), 0);
        WKPageSetPageLoaderClient(m_mainWebView->page(), 0);
        WKPageSetPagePolicyClient(m_mainWebView->page(), 0);
        WKPageClose(m_mainWebView->page());
        
        m_mainWebView = nullptr;

        createWebViewWithOptions(options);
        resetStateToConsistentValues();
    }
}

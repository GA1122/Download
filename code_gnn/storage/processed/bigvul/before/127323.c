    void initializeTextSelectionWebView(const std::string& url, FrameTestHelpers::WebViewHelper* webViewHelper)
    {
        webViewHelper->initializeAndLoad(url, true);
        webViewHelper->webView()->settings()->setDefaultFontSize(12);
        webViewHelper->webView()->resize(WebSize(640, 480));
    }

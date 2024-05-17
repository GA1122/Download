    virtual void SetUp()
    {
        m_webViewImpl = toWebViewImpl(WebView::create(0));

        WebSettings* settings = m_webViewImpl->settings();
        settings->setImagesEnabled(true);
        settings->setLoadsImagesAutomatically(true);
        settings->setJavaScriptEnabled(true);

        m_mainFrame = WebFrame::create(&m_webFrameClient);
        m_webViewImpl->setMainFrame(m_mainFrame);
    }

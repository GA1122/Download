    LoadStartedCatcher(QQuickWebView* webView)
        : m_webView(webView)
    {
        connect(m_webView, SIGNAL(loadingChanged(QWebLoadRequest*)), this, SLOT(onLoadingChanged(QWebLoadRequest*)));
    }

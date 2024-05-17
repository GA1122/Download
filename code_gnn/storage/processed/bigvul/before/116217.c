QQuickWebViewExperimental::QQuickWebViewExperimental(QQuickWebView *webView)
    : QObject(webView)
    , q_ptr(webView)
    , d_ptr(webView->d_ptr.data())
    , schemeParent(new QObject(this))
    , m_test(new QWebKitTest(webView->d_ptr.data(), this))
{
}

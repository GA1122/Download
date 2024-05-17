void tst_QQuickWebView::loadEmptyUrl()
{
    webView()->setUrl(QUrl());
    webView()->setUrl(QUrl(QLatin1String("")));
}

void QQuickWebView::goForward()
{
    Q_D(QQuickWebView);
    d->webPageProxy->goForward();
}

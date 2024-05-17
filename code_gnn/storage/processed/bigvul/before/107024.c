bool QQuickWebView::loading() const
{
    Q_D(const QQuickWebView);
    RefPtr<WebKit::WebFrameProxy> mainFrame = d->webPageProxy->mainFrame();
    return mainFrame && !(WebFrameProxy::LoadStateFinished == mainFrame->loadState());
}

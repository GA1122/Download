int QQuickWebView::loadProgress() const
{
    Q_D(const QQuickWebView);
    return d->pageLoadClient->loadProgress();
}
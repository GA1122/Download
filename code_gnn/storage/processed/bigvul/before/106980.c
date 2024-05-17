void QQuickWebViewFlickablePrivate::didChangeContentsSize(const QSize& newSize)
{
    Q_Q(QQuickWebView);
    if (isTransitioningToNewPage) {
        postTransitionState->contentsSize = newSize;
        return;
    }

    pageView->setContentsSize(newSize);
    q->experimental()->viewportInfo()->didUpdateContentsSize();
}

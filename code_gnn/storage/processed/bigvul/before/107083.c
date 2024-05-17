void QQuickWebViewLegacyPrivate::updateViewportSize()
{
    Q_Q(QQuickWebView);
    QSize viewportSize = q->boundingRect().size().toSize();
    pageView->setContentsSize(viewportSize);
    webPageProxy->drawingArea()->setVisibleContentsRectForScaling(IntRect(IntPoint(), viewportSize), 1);
}

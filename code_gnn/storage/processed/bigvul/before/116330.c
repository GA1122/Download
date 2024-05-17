void QQuickWebViewLegacyPrivate::updateViewportSize()
{
    Q_Q(QQuickWebView);
    QSizeF viewportSize = q->boundingRect().size();
    if (viewportSize.isEmpty())
        return;
    pageView->setContentsSize(viewportSize);
    webPageProxy->drawingArea()->setSize(viewportSize.toSize(), IntSize());
    webPageProxy->drawingArea()->setVisibleContentsRect(FloatRect(FloatPoint(), viewportSize), 1, FloatPoint());
}

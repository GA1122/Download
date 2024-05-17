QRect QQuickWebViewPrivate::visibleContentsRect() const
{
    Q_Q(const QQuickWebView);
    const QRectF visibleRect(q->boundingRect().intersected(pageView->boundingRect()));

    return q->mapRectToWebContent(visibleRect).toAlignedRect();
}

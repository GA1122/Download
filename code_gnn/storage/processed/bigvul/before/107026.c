QRectF QQuickWebView::mapRectFromWebContent(const QRectF& rectInCSSCoordinates) const
{
    Q_D(const QQuickWebView);
    return d->pageView->transformToItem().mapRect(rectInCSSCoordinates);
}

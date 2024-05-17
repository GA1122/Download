QPointF QQuickWebViewFlickablePrivate::pageItemPos()
{
    qreal xPos = flickProvider->contentItem()->x() + pageView->x();
    qreal yPos = flickProvider->contentItem()->y() + pageView->y();
    return QPointF(xPos, yPos);
}

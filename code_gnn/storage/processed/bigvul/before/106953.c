void QQuickWebViewPrivate::_q_commitPositionChange(const QPointF& trajectoryVector)
{
    DrawingAreaProxy* drawingArea = webPageProxy->drawingArea();
    if (!drawingArea)
        return;

    const QRect visibleRect(visibleContentsRect());
    drawingArea->setVisibleContentsRectForPanning(visibleRect, trajectoryVector);

    if (!trajectoryVector.isNull())
        return;

    webPageProxy->setFixedVisibleContentRect(visibleRect);
}

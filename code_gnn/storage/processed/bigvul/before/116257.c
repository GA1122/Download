void QQuickWebView::handleFlickableMouseRelease(const QPointF& position, qint64 eventTimestampMillis)
{
    Q_D(QQuickWebView);
    QMouseEvent mouseEvent(QEvent::MouseButtonRelease, d->axisLocker.adjust(position), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    d->axisLocker.reset();
    mouseEvent.setTimestamp(eventTimestampMillis);
    QQuickFlickable::mouseReleaseEvent(&mouseEvent);
}

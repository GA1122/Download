void QQuickWebView::handleFlickableMousePress(const QPointF& position, qint64 eventTimestampMillis)
{
    Q_D(QQuickWebView);
    d->axisLocker.setReferencePosition(position);
    QMouseEvent mouseEvent(QEvent::MouseButtonPress, position, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    mouseEvent.setTimestamp(eventTimestampMillis);
    QQuickFlickable::mousePressEvent(&mouseEvent);
}

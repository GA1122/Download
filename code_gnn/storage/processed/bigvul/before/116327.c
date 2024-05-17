void QQuickWebViewPrivate::FlickableAxisLocker::update(const QTouchEvent* event)
{
    ASSERT(event->touchPoints().size() == 1);
    const QTouchEvent::TouchPoint& touchPoint = event->touchPoints().first();

    ++m_sampleCount;

    if (m_sampleCount == 1) {
        m_initialPosition = touchPoint.pos();
        m_time = event->timestamp();
        return;
    }

    if (m_sampleCount > kAxisLockSampleCount
            || m_allowedDirection == QQuickFlickable::HorizontalFlick
            || m_allowedDirection == QQuickFlickable::VerticalFlick)
        return;

    QVector2D velocity = touchVelocity(event);

    qreal directionIndicator = qAbs(velocity.x()) - qAbs(velocity.y());

    if (velocity.length() > kAxisLockVelocityThreshold && qAbs(directionIndicator) > kAxisLockVelocityDirectionThreshold)
        m_allowedDirection = (directionIndicator > 0) ? QQuickFlickable::HorizontalFlick : QQuickFlickable::VerticalFlick;
}

QPointF QQuickWebViewPrivate::FlickableAxisLocker::adjust(const QPointF& position)
{
    if (m_allowedDirection == QQuickFlickable::HorizontalFlick)
        return QPointF(position.x(), m_lockReferencePosition.y());

    if (m_allowedDirection == QQuickFlickable::VerticalFlick)
        return QPointF(m_lockReferencePosition.x(), position.y());

    return position;
}

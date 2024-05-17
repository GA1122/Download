QQuickWebViewPrivate::FlickableAxisLocker::FlickableAxisLocker()
    : m_allowedDirection(QQuickFlickable::AutoFlickDirection)
    , m_time(0), m_sampleCount(0)
{
}

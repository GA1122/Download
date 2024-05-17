const AffineTransform& LayoutSVGResourceMarker::localToSVGParentTransform() const
{
    m_localToParentTransform = AffineTransform::translation(m_viewport.x(), m_viewport.y()) * viewportTransform();
    return m_localToParentTransform;
}

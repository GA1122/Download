void CCLayerTreeHost::setZoomAnimatorTransform(const TransformationMatrix& zoom)
{
    bool zoomChanged = m_zoomAnimatorTransform != zoom;

    m_zoomAnimatorTransform = zoom;

    if (zoomChanged)
        setNeedsCommitThenRedraw();
}

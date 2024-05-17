void WebPagePrivate::setInspectorOverlayClient(InspectorOverlay::InspectorOverlayClient* inspectorOverlayClient)
{
    if (inspectorOverlayClient) {
        if (!m_inspectorOverlay)
            m_inspectorOverlay = InspectorOverlay::create(this, inspectorOverlayClient);
        else
            m_inspectorOverlay->setClient(inspectorOverlayClient);
        m_inspectorOverlay->update();
        scheduleRootLayerCommit();
    } else {
        if (m_inspectorOverlay) {
            m_inspectorOverlay->clear();
            m_inspectorOverlay = nullptr;
            scheduleRootLayerCommit();
        }
    }
}

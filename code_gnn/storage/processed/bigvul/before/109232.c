void InspectorOverlay::freePage()
{
    m_overlayPage.clear();
    m_overlayChromeClient.clear();
    m_timer.stop();
}

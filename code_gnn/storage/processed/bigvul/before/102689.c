void CCLayerTreeHost::setNeedsRedraw()
{
    if (m_settings.enableCompositorThread)
        m_proxy->setNeedsRedraw();
    else
        m_client->scheduleComposite();
}

void CCLayerTreeHost::setNeedsCommitThenRedraw()
{
    if (m_settings.enableCompositorThread) {
        TRACE_EVENT("CCLayerTreeHost::setNeedsRedraw", this, 0);
        m_proxy->setNeedsCommitThenRedraw();
    } else
        m_client->scheduleComposite();
}

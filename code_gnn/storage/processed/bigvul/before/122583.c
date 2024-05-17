void WebDevToolsAgentImpl::setTouchEventEmulationEnabled(bool enabled)
{
    m_client->setTouchEventEmulationEnabled(enabled, m_emulateViewportEnabled);
    m_touchEventEmulationEnabled = enabled;
}

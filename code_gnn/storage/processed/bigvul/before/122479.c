void InspectorController::reconnectFrontend()
{
    if (!m_inspectorFrontend)
        return;
    InspectorFrontendChannel* frontendChannel = m_inspectorFrontend->channel();
    disconnectFrontend();
    connectFrontend(frontendChannel);
}

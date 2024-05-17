void InspectorController::reconnectFrontend()
{
    if (!m_inspectorFrontend)
        return;
    InspectorFrontendChannel* frontendChannel = m_inspectorFrontend->inspector()->getInspectorFrontendChannel();
    disconnectFrontend();
    connectFrontend(frontendChannel);
}

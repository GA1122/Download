void InspectorController::webViewResized(const IntSize& size)
{
    if (InspectorPageAgent* pageAgent = m_instrumentingAgents->inspectorPageAgent())
        pageAgent->webViewResized(size);
}

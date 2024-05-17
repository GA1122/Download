bool InspectorController::deviceEmulationEnabled()
{
    if (InspectorPageAgent* pageAgent = m_instrumentingAgents->inspectorPageAgent())
        return pageAgent->deviceMetricsOverrideEnabled();
    return false;
}

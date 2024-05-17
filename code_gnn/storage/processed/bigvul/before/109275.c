void InspectorPageAgent::clearFrontend()
{
    ErrorString error;
    disable(&error);
    updateTouchEventEmulationInPage(false);
    m_frontend = 0;
}

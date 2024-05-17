void InspectorController::didClearWindowObjectInMainWorld(LocalFrame* frame)
{
    if (m_inspectorFrontendClient && frame == m_page->mainFrame())
        m_inspectorFrontendClient->windowObjectCleared();
}

bool InspectorController::handleTouchEvent(Frame* frame, const PlatformTouchEvent& event)
{
    m_overlay->handleTouchEvent(event);
    if (InspectorDOMAgent* domAgent = m_instrumentingAgents->inspectorDOMAgent())
        return domAgent->handleTouchEvent(frame, event);
    return false;
}

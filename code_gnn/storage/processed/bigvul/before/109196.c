bool InspectorController::handleGestureEvent(Frame* frame, const PlatformGestureEvent& event)
{
    m_overlay->handleGestureEvent(event);
    if (InspectorDOMAgent* domAgent = m_instrumentingAgents->inspectorDOMAgent())
        return domAgent->handleGestureEvent(frame, event);
    return false;
}

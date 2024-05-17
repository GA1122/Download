bool InspectorController::handleKeyboardEvent(Frame* frame, const PlatformKeyboardEvent& event)
{
    m_overlay->handleKeyboardEvent(event);
    return false;
}

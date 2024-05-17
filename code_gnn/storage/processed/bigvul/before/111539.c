void TouchEventHandler::handleFatFingerPressed(bool shiftActive, bool altActive, bool ctrlActive)
{
    PlatformMouseEvent mouseMoveEvent(m_webPage->mapFromContentsToViewport(m_lastFatFingersResult.adjustedPosition()), m_lastScreenPoint, PlatformEvent::MouseMoved, 0, LeftButton, shiftActive, ctrlActive, altActive, TouchScreen);
    m_webPage->handleMouseEvent(mouseMoveEvent);

    PlatformMouseEvent mousePressedEvent(m_webPage->mapFromContentsToViewport(m_lastFatFingersResult.adjustedPosition()), m_lastScreenPoint, PlatformEvent::MousePressed, 1, LeftButton, shiftActive, ctrlActive, altActive, TouchScreen);
    m_webPage->handleMouseEvent(mousePressedEvent);
}

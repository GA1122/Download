void TouchEventHandler::sendClickAtFatFingersPoint(unsigned modifiers)
{
    bool shiftActive = modifiers & KEYMOD_SHIFT;
    bool altActive = modifiers & KEYMOD_ALT;
    bool ctrlActive = modifiers & KEYMOD_CTRL;

    handleFatFingerPressed(shiftActive, altActive, ctrlActive);
    PlatformMouseEvent mouseRelease(m_webPage->mapFromContentsToViewport(m_lastFatFingersResult.adjustedPosition()), m_lastScreenPoint, PlatformEvent::MouseReleased, 1, LeftButton, shiftActive, ctrlActive, altActive, TouchScreen);
     m_webPage->handleMouseEvent(mouseRelease);
 }

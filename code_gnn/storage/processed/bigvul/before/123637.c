void DeviceOrientationController::didRemoveEventListener(DOMWindow* window, const AtomicString& eventType)
{
    if (eventType == EventTypeNames::deviceorientation) {
        stopUpdating();
        m_hasEventListener = false;
    }
}

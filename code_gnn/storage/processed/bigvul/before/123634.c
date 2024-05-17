void DeviceOrientationController::didAddEventListener(DOMWindow* window, const AtomicString& eventType)
{
    if (eventType == EventTypeNames::deviceorientation && RuntimeEnabledFeatures::deviceOrientationEnabled()) {
        if (page() && page()->visibilityState() == PageVisibilityStateVisible)
            startUpdating();
        m_hasEventListener = true;
    }
}

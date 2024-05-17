static inline WebCore::PlatformEvent::Type toWebCoreMouseEventType(const BlackBerry::Platform::MouseEvent::Type type)
{
    switch (type) {
    case BlackBerry::Platform::MouseEvent::MouseButtonDown:
        return WebCore::PlatformEvent::MousePressed;
    case Platform::MouseEvent::MouseButtonUp:
        return WebCore::PlatformEvent::MouseReleased;
    case Platform::MouseEvent::MouseMove:
    default:
        return WebCore::PlatformEvent::MouseMoved;
    }
}

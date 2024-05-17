void DeviceOrientationController::didRemoveAllEventListeners(DOMWindow* window)
{
    stopUpdating();
    m_hasEventListener = false;
}

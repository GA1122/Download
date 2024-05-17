PassRefPtr<Event> DeviceOrientationController::getLastEvent()
{
    return DeviceOrientationEvent::create(EventTypeNames::deviceorientation,
        DeviceOrientationDispatcher::instance().latestDeviceOrientationData());
}

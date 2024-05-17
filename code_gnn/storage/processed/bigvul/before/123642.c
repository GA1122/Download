void DeviceOrientationController::registerWithDispatcher()
{
    DeviceOrientationDispatcher::instance().addDeviceOrientationController(this);
}

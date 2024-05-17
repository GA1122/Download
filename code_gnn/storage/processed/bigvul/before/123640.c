bool DeviceOrientationController::hasLastData()
{
    return DeviceOrientationDispatcher::instance().latestDeviceOrientationData();
}

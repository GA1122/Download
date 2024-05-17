void DeviceOrientationController::didChangeDeviceOrientation(DeviceOrientationData* deviceOrientationData)
{
    dispatchDeviceEvent(DeviceOrientationEvent::create(EventTypeNames::deviceorientation, deviceOrientationData));
}

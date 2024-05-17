void Camera3Device::overrideResultForPrecaptureCancel(
 CameraMetadata *result, const AeTriggerCancelOverride_t &aeTriggerCancelOverride) {
 if (aeTriggerCancelOverride.applyAeLock) {
        assert(mDeviceVersion <= CAMERA_DEVICE_API_VERSION_3_2);
        result->update(ANDROID_CONTROL_AE_LOCK, &aeTriggerCancelOverride.aeLock, 1);
 }

 if (aeTriggerCancelOverride.applyAePrecaptureTrigger) {
        assert(mDeviceVersion <= CAMERA_DEVICE_API_VERSION_3_2);
        result->update(ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER,
 &aeTriggerCancelOverride.aePrecaptureTrigger, 1);
 }
}

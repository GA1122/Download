void Camera3Device::RequestThread::handleAePrecaptureCancelRequest(sp<CaptureRequest> request) {
    request->mAeTriggerCancelOverride.applyAeLock = false;
    request->mAeTriggerCancelOverride.applyAePrecaptureTrigger = false;

 if (mHal3Device->common.version > CAMERA_DEVICE_API_VERSION_3_2) {
 return;
 }

 camera_metadata_entry_t aePrecaptureTrigger =
            request->mSettings.find(ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER);
 if (aePrecaptureTrigger.count > 0 &&
            aePrecaptureTrigger.data.u8[0] == ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_CANCEL) {
 uint8_t aePrecaptureTrigger = ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_IDLE;
        request->mSettings.update(ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER, &aePrecaptureTrigger, 1);
        request->mAeTriggerCancelOverride.applyAePrecaptureTrigger = true;
        request->mAeTriggerCancelOverride.aePrecaptureTrigger =
                ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER_CANCEL;

 if (mAeLockAvailable == true) {
 camera_metadata_entry_t aeLock = request->mSettings.find(ANDROID_CONTROL_AE_LOCK);
 if (aeLock.count == 0 ||  aeLock.data.u8[0] == ANDROID_CONTROL_AE_LOCK_OFF) {
 uint8_t aeLock = ANDROID_CONTROL_AE_LOCK_ON;
                request->mSettings.update(ANDROID_CONTROL_AE_LOCK, &aeLock, 1);
                request->mAeTriggerCancelOverride.applyAeLock = true;
                request->mAeTriggerCancelOverride.aeLock = ANDROID_CONTROL_AE_LOCK_OFF;
 }
 }
 }
}

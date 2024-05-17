CameraService::CameraService()
 :mSoundRef(0), mModule(0)
{
    ALOGI("CameraService started (pid=%d)", getpid());
    gCameraService = this;

 for (size_t i = 0; i < MAX_CAMERAS; ++i) {
        mStatusList[i] = ICameraServiceListener::STATUS_PRESENT;
 }

 this->camera_device_status_change = android::camera_device_status_change;
}

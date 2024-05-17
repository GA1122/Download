void CameraService::onFirstRef()
{
    LOG1("CameraService::onFirstRef");

 BnCameraService::onFirstRef();

 if (hw_get_module(CAMERA_HARDWARE_MODULE_ID,
 (const hw_module_t **)&mModule) < 0) {
        ALOGE("Could not load camera HAL module");
        mNumberOfCameras = 0;
 }
 else {
        ALOGI("Loaded \"%s\" camera module", mModule->common.name);
        mNumberOfCameras = mModule->get_number_of_cameras();
 if (mNumberOfCameras > MAX_CAMERAS) {
            ALOGE("Number of cameras(%d) > MAX_CAMERAS(%d).",
                    mNumberOfCameras, MAX_CAMERAS);
            mNumberOfCameras = MAX_CAMERAS;
 }
 for (int i = 0; i < mNumberOfCameras; i++) {
            setCameraFree(i);
 }

 if (mModule->common.module_api_version >=
                CAMERA_MODULE_API_VERSION_2_1) {
            mModule->set_callbacks(this);
 }

 CameraDeviceFactory::registerService(this);
 }
}

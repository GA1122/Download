status_t Camera3Device::initialize(CameraModule *module)
{
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);

    ALOGV("%s: Initializing device for camera %d", __FUNCTION__, mId);
 if (mStatus != STATUS_UNINITIALIZED) {
        CLOGE("Already initialized!");
 return INVALID_OPERATION;
 }

  

 status_t res;
 String8 deviceName = String8::format("%d", mId);

 camera3_device_t *device;

    ATRACE_BEGIN("camera3->open");
    res = module->open(deviceName.string(),
 reinterpret_cast<hw_device_t**>(&device));
    ATRACE_END();

 if (res != OK) {
        SET_ERR_L("Could not open camera: %s (%d)", strerror(-res), res);
 return res;
 }

  
 if (device->common.version < CAMERA_DEVICE_API_VERSION_3_0) {
        SET_ERR_L("Could not open camera: "
 "Camera device should be at least %x, reports %x instead",
                CAMERA_DEVICE_API_VERSION_3_0,
                device->common.version);
        device->common.close(&device->common);
 return BAD_VALUE;
 }

    camera_info info;
    res = CameraService::filterGetInfoErrorCode(module->getCameraInfo(
        mId, &info));
 if (res != OK) return res;

 if (info.device_version != device->common.version) {
        SET_ERR_L("HAL reporting mismatched camera_info version (%x)"
 " and device version (%x).",
                info.device_version, device->common.version);
        device->common.close(&device->common);
 return BAD_VALUE;
 }

  

    ATRACE_BEGIN("camera3->initialize");
    res = device->ops->initialize(device, this);
    ATRACE_END();

 if (res != OK) {
        SET_ERR_L("Unable to initialize HAL device: %s (%d)",
                strerror(-res), res);
        device->common.close(&device->common);
 return BAD_VALUE;
 }

  
    mStatusTracker = new StatusTracker(this);
    res = mStatusTracker->run(String8::format("C3Dev-%d-Status", mId).string());
 if (res != OK) {
        SET_ERR_L("Unable to start status tracking thread: %s (%d)",
                strerror(-res), res);
        device->common.close(&device->common);
        mStatusTracker.clear();
 return res;
 }

 bool aeLockAvailable = false;
    camera_metadata_ro_entry aeLockAvailableEntry;
    res = find_camera_metadata_ro_entry(info.static_camera_characteristics,
            ANDROID_CONTROL_AE_LOCK_AVAILABLE, &aeLockAvailableEntry);
 if (res == OK && aeLockAvailableEntry.count > 0) {
        aeLockAvailable = (aeLockAvailableEntry.data.u8[0] ==
                ANDROID_CONTROL_AE_LOCK_AVAILABLE_TRUE);
 }

  
    mRequestThread = new RequestThread(this, mStatusTracker, device, aeLockAvailable);
    res = mRequestThread->run(String8::format("C3Dev-%d-ReqQueue", mId).string());
 if (res != OK) {
        SET_ERR_L("Unable to start request queue thread: %s (%d)",
                strerror(-res), res);
        device->common.close(&device->common);
        mRequestThread.clear();
 return res;
 }

    mPreparerThread = new PreparerThread();

  

    mDeviceVersion = device->common.version;
    mDeviceInfo = info.static_camera_characteristics;
    mHal3Device = device;

    internalUpdateStatusLocked(STATUS_UNCONFIGURED);
    mNextStreamId = 0;
    mDummyStreamId = NO_STREAM;
    mNeedConfig = true;
    mPauseStateNotify = false;

 if (mDeviceVersion >= CAMERA_DEVICE_API_VERSION_3_2) {
        camera_metadata_entry partialResultsCount =
                mDeviceInfo.find(ANDROID_REQUEST_PARTIAL_RESULT_COUNT);
 if (partialResultsCount.count > 0) {
            mNumPartialResults = partialResultsCount.data.i32[0];
            mUsePartialResult = (mNumPartialResults > 1);
 }
 } else {
        camera_metadata_entry partialResultsQuirk =
                mDeviceInfo.find(ANDROID_QUIRKS_USE_PARTIAL_RESULT);
 if (partialResultsQuirk.count > 0 && partialResultsQuirk.data.u8[0] == 1) {
            mUsePartialResult = true;
 }
 }

    camera_metadata_entry configs =
            mDeviceInfo.find(ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS);
 for (uint32_t i = 0; i < configs.count; i += 4) {
 if (configs.data.i32[i] == HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED &&
                configs.data.i32[i + 3] ==
                ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_INPUT) {
            mSupportedOpaqueInputSizes.add(Size(configs.data.i32[i + 1],
                    configs.data.i32[i + 2]));
 }
 }

 return OK;
}

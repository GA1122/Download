const CameraMetadata& Camera3Device::info() const {
    ALOGVV("%s: E", __FUNCTION__);
 if (CC_UNLIKELY(mStatus == STATUS_UNINITIALIZED ||
                    mStatus == STATUS_ERROR)) {
        ALOGW("%s: Access to static info %s!", __FUNCTION__,
                mStatus == STATUS_ERROR ?
 "when in error state" : "before init");
 }
 return mDeviceInfo;
}

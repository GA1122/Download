status_t Camera3Device::waitUntilDrainedLocked() {
 switch (mStatus) {
 case STATUS_UNINITIALIZED:
 case STATUS_UNCONFIGURED:
            ALOGV("%s: Already idle", __FUNCTION__);
 return OK;
 case STATUS_CONFIGURED:
 case STATUS_ERROR:
 case STATUS_ACTIVE:
 break;
 default:
            SET_ERR_L("Unexpected status: %d",mStatus);
 return INVALID_OPERATION;
 }

    ALOGV("%s: Camera %d: Waiting until idle", __FUNCTION__, mId);
 status_t res = waitUntilStateThenRelock(  false, kShutdownTimeout);
 if (res != OK) {
        SET_ERR_L("Error waiting for HAL to drain: %s (%d)", strerror(-res),
                res);
 }
 return res;
}

status_t Camera3Device::internalPauseAndWaitLocked() {
    mRequestThread->setPaused(true);
    mPauseStateNotify = true;

    ALOGV("%s: Camera %d: Internal wait until idle", __FUNCTION__, mId);
 status_t res = waitUntilStateThenRelock(  false, kShutdownTimeout);
 if (res != OK) {
        SET_ERR_L("Can't idle device in %f seconds!",
                kShutdownTimeout/1e9);
 }

 return res;
}

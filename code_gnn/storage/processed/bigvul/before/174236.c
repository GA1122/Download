status_t Camera3Device::internalResumeLocked() {
 status_t res;

    mRequestThread->setPaused(false);

    res = waitUntilStateThenRelock(  true, kActiveTimeout);
 if (res != OK) {
        SET_ERR_L("Can't transition to active in %f seconds!",
                kActiveTimeout/1e9);
 }
    mPauseStateNotify = false;
 return OK;
}

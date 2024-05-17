status_t OMXCodec::init() {

    CHECK_EQ((int)mState, (int)LOADED);

 status_t err;
 if (!(mQuirks & kRequiresLoadedToIdleAfterAllocation)) {
        err = mOMX->sendCommand(mNode, OMX_CommandStateSet, OMX_StateIdle);
        CHECK_EQ(err, (status_t)OK);
        setState(LOADED_TO_IDLE);
 }

    err = allocateBuffers();
 if (err != (status_t)OK) {
 return err;
 }

 if (mQuirks & kRequiresLoadedToIdleAfterAllocation) {
        err = mOMX->sendCommand(mNode, OMX_CommandStateSet, OMX_StateIdle);
        CHECK_EQ(err, (status_t)OK);

        setState(LOADED_TO_IDLE);
 }

 while (mState != EXECUTING && mState != ERROR) {
        mAsyncCompletion.wait(mLock);
 }

 return mState == ERROR ? UNKNOWN_ERROR : OK;
}

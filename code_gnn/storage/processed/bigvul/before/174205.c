status_t Camera3Device::configureStreamsLocked() {
    ATRACE_CALL();
 status_t res;

 if (mStatus != STATUS_UNCONFIGURED && mStatus != STATUS_CONFIGURED) {
        CLOGE("Not idle");
 return INVALID_OPERATION;
 }

 if (!mNeedConfig) {
        ALOGV("%s: Skipping config, no stream changes", __FUNCTION__);
 return OK;
 }

 if (mOutputStreams.size() == 0) {
        addDummyStreamLocked();
 } else {
        tryRemoveDummyStreamLocked();
 }

    ALOGV("%s: Camera %d: Starting stream configuration", __FUNCTION__, mId);

    camera3_stream_configuration config;
    config.operation_mode = mIsConstrainedHighSpeedConfiguration ?
            CAMERA3_STREAM_CONFIGURATION_CONSTRAINED_HIGH_SPEED_MODE :
            CAMERA3_STREAM_CONFIGURATION_NORMAL_MODE;
    config.num_streams = (mInputStream != NULL) + mOutputStreams.size();

 Vector<camera3_stream_t*> streams;
    streams.setCapacity(config.num_streams);

 if (mInputStream != NULL) {
 camera3_stream_t *inputStream;
        inputStream = mInputStream->startConfiguration();
 if (inputStream == NULL) {
            SET_ERR_L("Can't start input stream configuration");
 return INVALID_OPERATION;
 }
        streams.add(inputStream);
 }

 for (size_t i = 0; i < mOutputStreams.size(); i++) {

 if (mOutputStreams[i].get() ==
 static_cast<Camera3StreamInterface*>(mInputStream.get())) {

            config.num_streams--;
 continue;
 }

 camera3_stream_t *outputStream;
        outputStream = mOutputStreams.editValueAt(i)->startConfiguration();
 if (outputStream == NULL) {
            SET_ERR_L("Can't start output stream configuration");
 return INVALID_OPERATION;
 }
        streams.add(outputStream);
 }

    config.streams = streams.editArray();

    ATRACE_BEGIN("camera3->configure_streams");
    res = mHal3Device->ops->configure_streams(mHal3Device, &config);
    ATRACE_END();

 if (res == BAD_VALUE) {
 if (mInputStream != NULL && mInputStream->isConfiguring()) {
            res = mInputStream->cancelConfiguration();
 if (res != OK) {
                SET_ERR_L("Can't cancel configuring input stream %d: %s (%d)",
                        mInputStream->getId(), strerror(-res), res);
 return res;
 }
 }

 for (size_t i = 0; i < mOutputStreams.size(); i++) {
            sp<Camera3OutputStreamInterface> outputStream =
                    mOutputStreams.editValueAt(i);
 if (outputStream->isConfiguring()) {
                res = outputStream->cancelConfiguration();
 if (res != OK) {
                    SET_ERR_L(
 "Can't cancel configuring output stream %d: %s (%d)",
                        outputStream->getId(), strerror(-res), res);
 return res;
 }
 }
 }

        internalUpdateStatusLocked(STATUS_UNCONFIGURED);
        mNeedConfig = true;

        ALOGV("%s: Camera %d: Stream configuration failed", __FUNCTION__, mId);
 return BAD_VALUE;
 } else if (res != OK) {
        SET_ERR_L("Unable to configure streams with HAL: %s (%d)",
                strerror(-res), res);
 return res;
 }


 if (mInputStream != NULL && mInputStream->isConfiguring()) {
        res = mInputStream->finishConfiguration(mHal3Device);
 if (res != OK) {
            SET_ERR_L("Can't finish configuring input stream %d: %s (%d)",
                    mInputStream->getId(), strerror(-res), res);
 return res;
 }
 }

 for (size_t i = 0; i < mOutputStreams.size(); i++) {
        sp<Camera3OutputStreamInterface> outputStream =
            mOutputStreams.editValueAt(i);
 if (outputStream->isConfiguring()) {
            res = outputStream->finishConfiguration(mHal3Device);
 if (res != OK) {
                SET_ERR_L("Can't finish configuring output stream %d: %s (%d)",
                        outputStream->getId(), strerror(-res), res);
 return res;
 }
 }
 }

    mRequestThread->configurationComplete();

 if (mIsConstrainedHighSpeedConfiguration) {
 pid_t requestThreadTid = mRequestThread->getTid();
        res = requestPriority(getpid(), requestThreadTid,
                kConstrainedHighSpeedThreadPriority, true);
 if (res != OK) {
            ALOGW("Can't set realtime priority for request processing thread: %s (%d)",
                    strerror(-res), res);
 } else {
            ALOGD("Set real time priority for request queue thread (tid %d)", requestThreadTid);
 }
 } else {
 }


    mNeedConfig = false;

    internalUpdateStatusLocked((mDummyStreamId == NO_STREAM) ?
            STATUS_CONFIGURED : STATUS_UNCONFIGURED);

    ALOGV("%s: Camera %d: Stream configuration complete", __FUNCTION__, mId);

    mDeletedStreams.clear();

 return OK;
}

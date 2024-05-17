status_t OMXCodec::start(MetaData *meta) {
 Mutex::Autolock autoLock(mLock);

 if (mState != LOADED) {
        CODEC_LOGE("called start in the unexpected state: %d", mState);
 return UNKNOWN_ERROR;
 }

    sp<MetaData> params = new MetaData;
 if (mQuirks & kWantsNALFragments) {
        params->setInt32(kKeyWantsNALFragments, true);
 }
 if (meta) {
 int64_t startTimeUs = 0;
 int64_t timeUs;
 if (meta->findInt64(kKeyTime, &timeUs)) {
            startTimeUs = timeUs;
 }
        params->setInt64(kKeyTime, startTimeUs);
 }

    mCodecSpecificDataIndex = 0;
    mInitialBufferSubmit = true;
    mSignalledEOS = false;
    mNoMoreOutputData = false;
    mOutputPortSettingsHaveChanged = false;
    mSeekTimeUs = -1;
    mSeekMode = ReadOptions::SEEK_CLOSEST_SYNC;
    mTargetTimeUs = -1;
    mFilledBuffers.clear();
    mPaused = false;

 status_t err;
 if (mIsEncoder) {
 if ((err = init()) != OK) {
            CODEC_LOGE("init failed: %d", err);
 return err;
 }

        params->setInt32(kKeyNumBuffers, mPortBuffers[kPortIndexInput].size());
        err = mSource->start(params.get());
 if (err != OK) {
            CODEC_LOGE("source failed to start: %d", err);
            stopOmxComponent_l();
 }
 return err;
 }

 if ((err = mSource->start(params.get())) != OK) {
        CODEC_LOGE("source failed to start: %d", err);
 return err;
 }
 return init();
}

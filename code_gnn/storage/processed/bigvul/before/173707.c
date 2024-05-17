status_t AudioSource::start(MetaData *params) {
 Mutex::Autolock autoLock(mLock);
 if (mStarted) {
 return UNKNOWN_ERROR;
 }

 if (mInitCheck != OK) {
 return NO_INIT;
 }

    mTrackMaxAmplitude = false;
    mMaxAmplitude = 0;
    mInitialReadTimeUs = 0;
    mStartTimeUs = 0;
 int64_t startTimeUs;
 if (params && params->findInt64(kKeyTime, &startTimeUs)) {
        mStartTimeUs = startTimeUs;
 }
 status_t err = mRecord->start();
 if (err == OK) {
        mStarted = true;
 } else {
        mRecord.clear();
 }


 return err;
}

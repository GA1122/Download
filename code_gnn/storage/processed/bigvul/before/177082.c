status_t SoftMPEG2::resetPlugin() {
    mIsInFlush = false;
    mReceivedEOS = false;
    memset(mTimeStamps, 0, sizeof(mTimeStamps));
    memset(mTimeStampsValid, 0, sizeof(mTimeStampsValid));

  
    gettimeofday(&mTimeStart, NULL);
    gettimeofday(&mTimeEnd, NULL);

 return OK;
}

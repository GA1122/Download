status_t MediaPlayerService::AudioOutput::setPlaybackRate(const AudioPlaybackRate &rate)
{
    ALOGV("setPlaybackRate(%f %f %d %d)",
                rate.mSpeed, rate.mPitch, rate.mFallbackMode, rate.mStretchMode);
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) {
        mPlaybackRate = rate;
 return OK;
 }
 status_t res = mTrack->setPlaybackRate(rate);
 if (res != NO_ERROR) {
 return res;
 }
    CHECK_GT(rate.mSpeed, 0.f);
    mPlaybackRate = rate;
 if (mSampleRateHz != 0) {
        mMsecsPerFrame = 1E3f / (rate.mSpeed * mSampleRateHz);
 }
 return res;
}

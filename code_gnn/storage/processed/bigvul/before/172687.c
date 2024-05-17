status_t MediaPlayer::setPlaybackSettings(const AudioPlaybackRate& rate)
{
    ALOGV("setPlaybackSettings: %f %f %d %d",
            rate.mSpeed, rate.mPitch, rate.mFallbackMode, rate.mStretchMode);
 if (rate.mSpeed < 0.f || rate.mPitch < 0.f) {
 return BAD_VALUE;
 }
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 status_t err = mPlayer->setPlaybackSettings(rate);
 if (err == OK) {
 if (rate.mSpeed == 0.f && mCurrentState == MEDIA_PLAYER_STARTED) {
            mCurrentState = MEDIA_PLAYER_PAUSED;
 } else if (rate.mSpeed != 0.f && mCurrentState == MEDIA_PLAYER_PAUSED) {
            mCurrentState = MEDIA_PLAYER_STARTED;
 }
 }
 return err;
}

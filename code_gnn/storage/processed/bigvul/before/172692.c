status_t MediaPlayer::start()
{
    ALOGV("start");

 status_t ret = NO_ERROR;
 Mutex::Autolock _l(mLock);

    mLockThreadId = getThreadId();

 if (mCurrentState & MEDIA_PLAYER_STARTED) {
        ret = NO_ERROR;
 } else if ( (mPlayer != 0) && ( mCurrentState & ( MEDIA_PLAYER_PREPARED |
                    MEDIA_PLAYER_PLAYBACK_COMPLETE | MEDIA_PLAYER_PAUSED ) ) ) {
        mPlayer->setLooping(mLoop);
        mPlayer->setVolume(mLeftVolume, mRightVolume);
        mPlayer->setAuxEffectSendLevel(mSendLevel);
        mCurrentState = MEDIA_PLAYER_STARTED;
        ret = mPlayer->start();
 if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
 } else {
 if (mCurrentState == MEDIA_PLAYER_PLAYBACK_COMPLETE) {
                ALOGV("playback completed immediately following start()");
 }
 }
 } else {
        ALOGE("start called in state %d", mCurrentState);
        ret = INVALID_OPERATION;
 }

    mLockThreadId = 0;

 return ret;
}

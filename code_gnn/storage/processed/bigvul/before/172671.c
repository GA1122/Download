status_t MediaPlayer::pause()
{
    ALOGV("pause");
 Mutex::Autolock _l(mLock);
 if (mCurrentState & (MEDIA_PLAYER_PAUSED|MEDIA_PLAYER_PLAYBACK_COMPLETE))
 return NO_ERROR;
 if ((mPlayer != 0) && (mCurrentState & MEDIA_PLAYER_STARTED)) {
 status_t ret = mPlayer->pause();
 if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
 } else {
            mCurrentState = MEDIA_PLAYER_PAUSED;
 }
 return ret;
 }
    ALOGE("pause called in state %d", mCurrentState);
 return INVALID_OPERATION;
}

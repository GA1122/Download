status_t MediaPlayer::stop()
{
    ALOGV("stop");
 Mutex::Autolock _l(mLock);
 if (mCurrentState & MEDIA_PLAYER_STOPPED) return NO_ERROR;
 if ( (mPlayer != 0) && ( mCurrentState & ( MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PREPARED |
                    MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE ) ) ) {
 status_t ret = mPlayer->stop();
 if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
 } else {
            mCurrentState = MEDIA_PLAYER_STOPPED;
 }
 return ret;
 }
    ALOGE("stop called in state %d", mCurrentState);
 return INVALID_OPERATION;
}

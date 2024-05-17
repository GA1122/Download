bool MediaPlayer::isPlaying()
{
 Mutex::Autolock _l(mLock);
 if (mPlayer != 0) {
 bool temp = false;
        mPlayer->isPlaying(&temp);
        ALOGV("isPlaying: %d", temp);
 if ((mCurrentState & MEDIA_PLAYER_STARTED) && ! temp) {
            ALOGE("internal/external state mismatch corrected");
            mCurrentState = MEDIA_PLAYER_PAUSED;
 } else if ((mCurrentState & MEDIA_PLAYER_PAUSED) && temp) {
            ALOGE("internal/external state mismatch corrected");
            mCurrentState = MEDIA_PLAYER_STARTED;
 }
 return temp;
 }
    ALOGV("isPlaying: no active player");
 return false;
}

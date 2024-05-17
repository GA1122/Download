status_t MediaPlayer::getCurrentPosition(int *msec)
{
    ALOGV("getCurrentPosition");
 Mutex::Autolock _l(mLock);
 if (mPlayer != 0) {
 if (mCurrentPosition >= 0) {
            ALOGV("Using cached seek position: %d", mCurrentPosition);
 *msec = mCurrentPosition;
 return NO_ERROR;
 }
 return mPlayer->getCurrentPosition(msec);
 }
 return INVALID_OPERATION;
}

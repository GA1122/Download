status_t MediaPlayer::getVideoHeight(int *h)
{
    ALOGV("getVideoHeight");
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 *h = mVideoHeight;
 return NO_ERROR;
}

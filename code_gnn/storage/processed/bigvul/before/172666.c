status_t MediaPlayer::getVideoWidth(int *w)
{
    ALOGV("getVideoWidth");
 Mutex::Autolock _l(mLock);
 if (mPlayer == 0) return INVALID_OPERATION;
 *w = mVideoWidth;
 return NO_ERROR;
}

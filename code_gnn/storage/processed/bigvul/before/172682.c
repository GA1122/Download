status_t MediaPlayer::setListener(const sp<MediaPlayerListener>& listener)
{
    ALOGV("setListener");
 Mutex::Autolock _l(mLock);
    mListener = listener;
 return NO_ERROR;
}

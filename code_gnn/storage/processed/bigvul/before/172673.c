status_t MediaPlayer::prepareAsync()
{
    ALOGV("prepareAsync");
 Mutex::Autolock _l(mLock);
 return prepareAsync_l();
}

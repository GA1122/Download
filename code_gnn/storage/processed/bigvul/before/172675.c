status_t MediaPlayer::reset()
{
    ALOGV("reset");
 Mutex::Autolock _l(mLock);
 return reset_l();
}

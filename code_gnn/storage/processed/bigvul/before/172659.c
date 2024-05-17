status_t MediaPlayer::getDuration(int *msec)
{
 Mutex::Autolock _l(mLock);
 return getDuration_l(msec);
}

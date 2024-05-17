status_t MediaPlayer::seekTo(int msec)
{
    mLockThreadId = getThreadId();
 Mutex::Autolock _l(mLock);
 status_t result = seekTo_l(msec);
    mLockThreadId = 0;

 return result;
}

status_t MediaPlayer::setLooping(int loop)
{
    ALOGV("MediaPlayer::setLooping");
 Mutex::Autolock _l(mLock);
    mLoop = (loop != 0);
 if (mPlayer != 0) {
 return mPlayer->setLooping(loop);
 }
 return OK;
}

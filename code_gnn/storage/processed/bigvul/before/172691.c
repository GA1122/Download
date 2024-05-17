status_t MediaPlayer::setVolume(float leftVolume, float rightVolume)
{
    ALOGV("MediaPlayer::setVolume(%f, %f)", leftVolume, rightVolume);
 Mutex::Autolock _l(mLock);
    mLeftVolume = leftVolume;
    mRightVolume = rightVolume;
 if (mPlayer != 0) {
 return mPlayer->setVolume(leftVolume, rightVolume);
 }
 return OK;
}

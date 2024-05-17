void SoundChannel::pause()
{
 Mutex::Autolock lock(&mLock);
 if (mState == PLAYING) {
        ALOGV("pause track");
        mState = PAUSED;
        mAudioTrack->pause();
 }
}

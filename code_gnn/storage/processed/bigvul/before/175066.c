void SoundChannel::autoResume()
{
 Mutex::Autolock lock(&mLock);
 if (mAutoPaused && (mState == PAUSED)) {
        ALOGV("resume track");
        mState = PLAYING;
        mAutoPaused = false;
        mAudioTrack->start();
 }
}

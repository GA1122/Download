void SoundChannel::resume()
{
 Mutex::Autolock lock(&mLock);
 if (mState == PAUSED) {
        ALOGV("resume track");
        mState = PLAYING;
        mAutoPaused = false;
        mAudioTrack->start();
 }
}

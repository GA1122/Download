bool SoundChannel::doStop_l()
{
 if (mState != IDLE) {
        setVolume_l(0, 0);
        ALOGV("stop");
        mAudioTrack->stop();
        mSample.clear();
        mState = IDLE;
        mPriority = IDLE_PRIORITY;
 return true;
 }
 return false;
}

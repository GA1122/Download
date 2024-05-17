SoundChannel::~SoundChannel()
{
    ALOGV("SoundChannel destructor %p", this);
 {
 Mutex::Autolock lock(&mLock);
        clearNextEvent();
        doStop_l();
 }
    mAudioTrack.clear();
}

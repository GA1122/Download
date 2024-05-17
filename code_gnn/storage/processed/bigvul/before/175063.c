void SoundPool::autoPause()
{
    ALOGV("autoPause()");
 Mutex::Autolock lock(&mLock);
 for (int i = 0; i < mMaxChannels; ++i) {
 SoundChannel* channel = &mChannelPool[i];
        channel->autoPause();
 }
}

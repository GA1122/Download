void SoundPool::autoResume()
{
    ALOGV("autoResume()");
 Mutex::Autolock lock(&mLock);
 for (int i = 0; i < mMaxChannels; ++i) {
 SoundChannel* channel = &mChannelPool[i];
        channel->autoResume();
 }
}

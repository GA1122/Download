void SoundPool::pause(int channelID)
{
    ALOGV("pause(%d)", channelID);
 Mutex::Autolock lock(&mLock);
 SoundChannel* channel = findChannel(channelID);
 if (channel) {
        channel->pause();
 }
}

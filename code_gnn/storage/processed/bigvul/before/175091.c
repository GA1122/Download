void SoundPool::setPriority(int channelID, int priority)
{
    ALOGV("setPriority(%d, %d)", channelID, priority);
 Mutex::Autolock lock(&mLock);
 SoundChannel* channel = findChannel(channelID);
 if (channel) {
        channel->setPriority(priority);
 }
}

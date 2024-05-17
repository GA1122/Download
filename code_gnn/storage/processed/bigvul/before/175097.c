void SoundPool::stop(int channelID)
{
    ALOGV("stop(%d)", channelID);
 Mutex::Autolock lock(&mLock);
 SoundChannel* channel = findChannel(channelID);
 if (channel) {
        channel->stop();
 } else {
        channel = findNextChannel(channelID);
 if (channel)
            channel->clearNextEvent();
 }
}

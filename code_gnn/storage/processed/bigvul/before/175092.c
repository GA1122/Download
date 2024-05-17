void SoundPool::setRate(int channelID, float rate)
{
    ALOGV("setRate(%d, %f)", channelID, rate);
 Mutex::Autolock lock(&mLock);
 SoundChannel* channel = findChannel(channelID);
 if (channel) {
        channel->setRate(rate);
 }
}

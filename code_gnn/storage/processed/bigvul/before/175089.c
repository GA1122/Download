void SoundPool::setLoop(int channelID, int loop)
{
    ALOGV("setLoop(%d, %d)", channelID, loop);
 Mutex::Autolock lock(&mLock);
 SoundChannel* channel = findChannel(channelID);
 if (channel) {
        channel->setLoop(loop);
 }
}

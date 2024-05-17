SoundChannel* SoundPool::findNextChannel(int channelID)
{
 for (int i = 0; i < mMaxChannels; ++i) {
 if (mChannelPool[i].nextChannelID() == channelID) {
 return &mChannelPool[i];
 }
 }
 return NULL;
}

 SoundChannel* SoundPool::findChannel(int channelID)
 {
     for (int i = 0; i < mMaxChannels; ++i) {
 if (mChannelPool[i].channelID() == channelID) {
 return &mChannelPool[i];
 }
 }
 return NULL;
}

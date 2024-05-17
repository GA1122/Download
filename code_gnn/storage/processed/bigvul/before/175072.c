void SoundPool::dump()
{
 for (int i = 0; i < mMaxChannels; ++i) {
        mChannelPool[i].dump();
 }
}

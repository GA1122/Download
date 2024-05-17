SoundChannel* SoundPool::allocateChannel_l(int priority)
{
 List<SoundChannel*>::iterator iter;
 SoundChannel* channel = NULL;

 if (!mChannels.empty()) {
        iter = mChannels.begin();
 if (priority >= (*iter)->priority()) {
            channel = *iter;
            mChannels.erase(iter);
            ALOGV("Allocated active channel");
 }
 }

 if (channel) {
        channel->setPriority(priority);
 for (iter = mChannels.begin(); iter != mChannels.end(); ++iter) {
 if (priority < (*iter)->priority()) {
 break;
 }
 }
        mChannels.insert(iter, channel);
 }
 return channel;
}

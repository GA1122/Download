void SoundPool::moveToFront_l(SoundChannel* channel)
{
 for (List<SoundChannel*>::iterator iter = mChannels.begin(); iter != mChannels.end(); ++iter) {
 if (*iter == channel) {
            mChannels.erase(iter);
            mChannels.push_front(channel);
 break;
 }
 }
}

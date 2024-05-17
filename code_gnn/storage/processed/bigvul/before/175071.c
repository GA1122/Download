void SoundPool::done_l(SoundChannel* channel)
{
    ALOGV("done_l(%d)", channel->channelID());
 if (channel->nextChannelID() != 0) {
        ALOGV("add to restart list");
        addToRestartList(channel);
 }

 else {
        ALOGV("move to front");
        moveToFront_l(channel);
 }
}

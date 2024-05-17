void SoundChannel::nextEvent()
{
    sp<Sample> sample;
 int nextChannelID;
 float leftVolume;
 float rightVolume;
 int priority;
 int loop;
 float rate;

 {
 Mutex::Autolock lock(&mLock);
        nextChannelID = mNextEvent.channelID();
 if (nextChannelID  == 0) {
            ALOGV("stolen channel has no event");
 return;
 }

        sample = mNextEvent.sample();
        leftVolume = mNextEvent.leftVolume();
        rightVolume = mNextEvent.rightVolume();
        priority = mNextEvent.priority();
        loop = mNextEvent.loop();
        rate = mNextEvent.rate();
 }

    ALOGV("Starting stolen channel %d -> %d", channelID(), nextChannelID);
    play(sample, nextChannelID, leftVolume, rightVolume, priority, loop, rate);
}

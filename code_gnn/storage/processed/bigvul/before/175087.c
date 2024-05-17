void SoundEvent::set(const sp<Sample>& sample, int channelID, float leftVolume,
 float rightVolume, int priority, int loop, float rate)
{
    mSample = sample;
    mChannelID = channelID;
    mLeftVolume = leftVolume;
    mRightVolume = rightVolume;
    mPriority = priority;
    mLoop = loop;
    mRate =rate;
}

void SoundChannel::play(const sp<Sample>& sample, int nextChannelID, float leftVolume,
 float rightVolume, int priority, int loop, float rate)
{
    sp<AudioTrack> oldTrack;
    sp<AudioTrack> newTrack;
 status_t status;

 {  
 Mutex::Autolock lock(&mLock);

        ALOGV("SoundChannel::play %p: sampleID=%d, channelID=%d, leftVolume=%f, rightVolume=%f,"
 " priority=%d, loop=%d, rate=%f",
 this, sample->sampleID(), nextChannelID, leftVolume, rightVolume,
                priority, loop, rate);

 if (mState != IDLE) {
            ALOGV("channel %d stolen - event queued for channel %d", channelID(), nextChannelID);
            mNextEvent.set(sample, nextChannelID, leftVolume, rightVolume, priority, loop, rate);
            stop_l();
 return;
 }

 size_t afFrameCount;
 uint32_t afSampleRate;
 audio_stream_type_t streamType = audio_attributes_to_stream_type(mSoundPool->attributes());
 if (AudioSystem::getOutputFrameCount(&afFrameCount, streamType) != NO_ERROR) {
            afFrameCount = kDefaultFrameCount;
 }
 if (AudioSystem::getOutputSamplingRate(&afSampleRate, streamType) != NO_ERROR) {
            afSampleRate = kDefaultSampleRate;
 }
 int numChannels = sample->numChannels();
 uint32_t sampleRate = uint32_t(float(sample->sampleRate()) * rate + 0.5);
 uint32_t totalFrames = (kDefaultBufferCount * afFrameCount * sampleRate) / afSampleRate;
 uint32_t bufferFrames = (totalFrames + (kDefaultBufferCount - 1)) / kDefaultBufferCount;
 size_t frameCount = 0;

 if (loop) {
            frameCount = sample->size()/numChannels/
 ((sample->format() == AUDIO_FORMAT_PCM_16_BIT) ? sizeof(int16_t) : sizeof(uint8_t));
 }

#ifndef USE_SHARED_MEM_BUFFER
 if(frameCount < totalFrames) {
            frameCount = totalFrames;
 }
#endif

 unsigned long toggle = mToggle ^ 1;
 void *userData = (void *)((unsigned long)this | toggle);
 audio_channel_mask_t channelMask = audio_channel_out_mask_from_count(numChannels);

#ifdef USE_SHARED_MEM_BUFFER
        newTrack = new AudioTrack(streamType, sampleRate, sample->format(),
                channelMask, sample->getIMemory(), AUDIO_OUTPUT_FLAG_FAST, callback, userData);
#else
        newTrack = new AudioTrack(streamType, sampleRate, sample->format(),
                channelMask, frameCount, AUDIO_OUTPUT_FLAG_FAST, callback, userData,
                bufferFrames);
#endif
        oldTrack = mAudioTrack;
        status = newTrack->initCheck();
 if (status != NO_ERROR) {
            ALOGE("Error creating AudioTrack");
 goto exit;
 }
        ALOGV("setVolume %p", newTrack.get());
        newTrack->setVolume(leftVolume, rightVolume);
        newTrack->setLoop(0, frameCount, loop);

        mToggle = toggle;
        mAudioTrack = newTrack;
        mPos = 0;
        mSample = sample;
        mChannelID = nextChannelID;
        mPriority = priority;
        mLoop = loop;
        mLeftVolume = leftVolume;
        mRightVolume = rightVolume;
        mNumChannels = numChannels;
        mRate = rate;
        clearNextEvent();
        mState = PLAYING;
        mAudioTrack->start();
        mAudioBufferSize = newTrack->frameCount()*newTrack->frameSize();
 }

exit:
    ALOGV("delete oldTrack %p", oldTrack.get());
 if (status != NO_ERROR) {
        mAudioTrack.clear();
 }
}

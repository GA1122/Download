void SoundChannel::setLoop(int loop)
{
 Mutex::Autolock lock(&mLock);
 if (mAudioTrack != NULL && mSample != 0) {
 uint32_t loopEnd = mSample->size()/mNumChannels/
 ((mSample->format() == AUDIO_FORMAT_PCM_16_BIT) ? sizeof(int16_t) : sizeof(uint8_t));
        mAudioTrack->setLoop(0, loopEnd, loop);
        mLoop = loop;
 }
}

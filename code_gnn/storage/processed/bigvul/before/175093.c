void SoundChannel::setRate(float rate)
{
 Mutex::Autolock lock(&mLock);
 if (mAudioTrack != NULL && mSample != 0) {
 uint32_t sampleRate = uint32_t(float(mSample->sampleRate()) * rate + 0.5);
        mAudioTrack->setSampleRate(sampleRate);
        mRate = rate;
 }
}

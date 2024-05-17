void OMXCodec::setG711Format(int32_t sampleRate, int32_t numChannels) {
    CHECK(!mIsEncoder);
    setRawAudioFormat(kPortIndexInput, sampleRate, numChannels);
}

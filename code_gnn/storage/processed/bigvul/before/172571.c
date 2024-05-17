SoftAAC2::~SoftAAC2() {
    aacDecoder_Close(mAACDecoder);
 delete[] mOutputDelayRingBuffer;
}

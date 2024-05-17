status_t Sample::doLoad()
{
 uint32_t sampleRate;
 int numChannels;
 audio_format_t format;
 status_t status;
    mHeap = new MemoryHeapBase(kDefaultHeapSize);

    ALOGV("Start decode");
 if (mUrl) {
        status = MediaPlayer::decode(
                NULL  ,
                mUrl,
 &sampleRate,
 &numChannels,
 &format,
                mHeap,
 &mSize);
 } else {
        status = MediaPlayer::decode(mFd, mOffset, mLength, &sampleRate, &numChannels, &format,
                                     mHeap, &mSize);
        ALOGV("close(%d)", mFd);
 ::close(mFd);
        mFd = -1;
 }
 if (status != NO_ERROR) {
        ALOGE("Unable to load sample: %s", mUrl);
 goto error;
 }
    ALOGV("pointer = %p, size = %zu, sampleRate = %u, numChannels = %d",
          mHeap->getBase(), mSize, sampleRate, numChannels);

 if (sampleRate > kMaxSampleRate) {
       ALOGE("Sample rate (%u) out of range", sampleRate);
       status = BAD_VALUE;
 goto error;
 }

 if ((numChannels < 1) || (numChannels > 2)) {
        ALOGE("Sample channel count (%d) out of range", numChannels);
        status = BAD_VALUE;
 goto error;
 }

    mData = new MemoryBase(mHeap, 0, mSize);
    mSampleRate = sampleRate;
    mNumChannels = numChannels;
    mFormat = format;
    mState = READY;
 return NO_ERROR;

error:
    mHeap.clear();
 return status;
}

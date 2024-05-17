MediaBuffer *FLACParser::readBuffer(bool doSeek, FLAC__uint64 sample)
{
    mWriteRequested = true;
    mWriteCompleted = false;
 if (doSeek) {
 if (!FLAC__stream_decoder_seek_absolute(mDecoder, sample)) {
            ALOGE("FLACParser::readBuffer seek to sample %lld failed", (long long)sample);
 return NULL;
 }
        ALOGV("FLACParser::readBuffer seek to sample %lld succeeded", (long long)sample);
 } else {
 if (!FLAC__stream_decoder_process_single(mDecoder)) {
            ALOGE("FLACParser::readBuffer process_single failed");
 return NULL;
 }
 }
 if (!mWriteCompleted) {
        ALOGV("FLACParser::readBuffer write did not complete");
 return NULL;
 }
 unsigned blocksize = mWriteHeader.blocksize;
 if (blocksize == 0 || blocksize > getMaxBlockSize()) {
        ALOGE("FLACParser::readBuffer write invalid blocksize %u", blocksize);
 return NULL;
 }
 if (mWriteHeader.sample_rate != getSampleRate() ||
        mWriteHeader.channels != getChannels() ||
        mWriteHeader.bits_per_sample != getBitsPerSample()) {
        ALOGE("FLACParser::readBuffer write changed parameters mid-stream: %d/%d/%d -> %d/%d/%d",
                getSampleRate(), getChannels(), getBitsPerSample(),
                mWriteHeader.sample_rate, mWriteHeader.channels, mWriteHeader.bits_per_sample);
 return NULL;
 }
    CHECK(mGroup != NULL);
 MediaBuffer *buffer;
 status_t err = mGroup->acquire_buffer(&buffer);
 if (err != OK) {
 return NULL;
 }
 size_t bufferSize = blocksize * getChannels() * sizeof(short);
    CHECK(bufferSize <= mMaxBufferSize);
 short *data = (short *) buffer->data();
    buffer->set_range(0, bufferSize);
 (*mCopy)(data, mWriteBuffer, blocksize, getChannels());
    CHECK(mWriteHeader.number_type == FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER);
    FLAC__uint64 sampleNumber = mWriteHeader.number.sample_number;
 int64_t timeUs = (1000000LL * sampleNumber) / getSampleRate();
    buffer->meta_data()->setInt64(kKeyTime, timeUs);
    buffer->meta_data()->setInt32(kKeyIsSyncFrame, 1);
 return buffer;
}

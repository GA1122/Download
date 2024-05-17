bool OMXCodec::drainInputBuffer(BufferInfo *info) {
 if (info != NULL) {
        CHECK_EQ((int)info->mStatus, (int)OWNED_BY_US);
 }

 if (mSignalledEOS) {
 return false;
 }

 if (mCodecSpecificDataIndex < mCodecSpecificData.size()) {
        CHECK(!(mFlags & kUseSecureInputBuffers));

 const CodecSpecificData *specific =
            mCodecSpecificData[mCodecSpecificDataIndex];

 size_t size = specific->mSize;

 if ((!strcasecmp(MEDIA_MIMETYPE_VIDEO_AVC, mMIME) ||
 !strcasecmp(MEDIA_MIMETYPE_VIDEO_HEVC, mMIME))
 && !(mQuirks & kWantsNALFragments)) {
 static const uint8_t kNALStartCode[4] =
 { 0x00, 0x00, 0x00, 0x01 };

            CHECK(info->mSize >= specific->mSize + 4);

            size += 4;

            memcpy(info->mData, kNALStartCode, 4);
            memcpy((uint8_t *)info->mData + 4,
                   specific->mData, specific->mSize);
 } else {
            CHECK(info->mSize >= specific->mSize);
            memcpy(info->mData, specific->mData, specific->mSize);
 }

        mNoMoreOutputData = false;

        CODEC_LOGV("calling emptyBuffer with codec specific data");

 status_t err = mOMX->emptyBuffer(
                mNode, info->mBuffer, 0, size,
                OMX_BUFFERFLAG_ENDOFFRAME | OMX_BUFFERFLAG_CODECCONFIG,
 0);
        CHECK_EQ(err, (status_t)OK);

        info->mStatus = OWNED_BY_COMPONENT;

 ++mCodecSpecificDataIndex;
 return true;
 }

 if (mPaused) {
 return false;
 }

 status_t err;

 bool signalEOS = false;
 int64_t timestampUs = 0;

 size_t offset = 0;
 int32_t n = 0;


 for (;;) {
 MediaBuffer *srcBuffer;
 if (mSeekTimeUs >= 0) {
 if (mLeftOverBuffer) {
                mLeftOverBuffer->release();
                mLeftOverBuffer = NULL;
 }

 MediaSource::ReadOptions options;
            options.setSeekTo(mSeekTimeUs, mSeekMode);

            mSeekTimeUs = -1;
            mSeekMode = ReadOptions::SEEK_CLOSEST_SYNC;
            mBufferFilled.signal();

            err = mSource->read(&srcBuffer, &options);

 if (err == OK) {
 int64_t targetTimeUs;
 if (srcBuffer->meta_data()->findInt64(
                            kKeyTargetTime, &targetTimeUs)
 && targetTimeUs >= 0) {
                    CODEC_LOGV("targetTimeUs = %lld us", (long long)targetTimeUs);
                    mTargetTimeUs = targetTimeUs;
 } else {
                    mTargetTimeUs = -1;
 }
 }
 } else if (mLeftOverBuffer) {
            srcBuffer = mLeftOverBuffer;
            mLeftOverBuffer = NULL;

            err = OK;
 } else {
            err = mSource->read(&srcBuffer);
 }

 if (err != OK) {
            signalEOS = true;
            mFinalStatus = err;
            mSignalledEOS = true;
            mBufferFilled.signal();
 break;
 }

 if (mFlags & kUseSecureInputBuffers) {
            info = findInputBufferByDataPointer(srcBuffer->data());
            CHECK(info != NULL);
 }

 size_t remainingBytes = info->mSize - offset;

 if (srcBuffer->range_length() > remainingBytes) {
 if (offset == 0) {
                CODEC_LOGE(
 "Codec's input buffers are too small to accomodate "
 "buffer read from source (info->mSize = %zu, srcLength = %zu)",
                     info->mSize, srcBuffer->range_length());

                srcBuffer->release();
                srcBuffer = NULL;

                setState(ERROR);
 return false;
 }

            mLeftOverBuffer = srcBuffer;
 break;
 }

 bool releaseBuffer = true;
 if (mFlags & kStoreMetaDataInVideoBuffers) {
                releaseBuffer = false;
                info->mMediaBuffer = srcBuffer;
 }

 if (mFlags & kUseSecureInputBuffers) {

                releaseBuffer = false;

                CHECK(info->mMediaBuffer == NULL);
                info->mMediaBuffer = srcBuffer;
 } else {
            CHECK(srcBuffer->data() != NULL) ;
            memcpy((uint8_t *)info->mData + offset,
 (const uint8_t *)srcBuffer->data()
 + srcBuffer->range_offset(),
                    srcBuffer->range_length());
 }

 int64_t lastBufferTimeUs;
        CHECK(srcBuffer->meta_data()->findInt64(kKeyTime, &lastBufferTimeUs));
        CHECK(lastBufferTimeUs >= 0);
 if (mIsEncoder && mIsVideo) {
            mDecodingTimeList.push_back(lastBufferTimeUs);
 }

 if (offset == 0) {
            timestampUs = lastBufferTimeUs;
 }

        offset += srcBuffer->range_length();

 if (!strcasecmp(MEDIA_MIMETYPE_AUDIO_VORBIS, mMIME)) {
            CHECK(!(mQuirks & kSupportsMultipleFramesPerInputBuffer));
            CHECK_GE(info->mSize, offset + sizeof(int32_t));

 int32_t numPageSamples;
 if (!srcBuffer->meta_data()->findInt32(
                        kKeyValidSamples, &numPageSamples)) {
                numPageSamples = -1;
 }

            memcpy((uint8_t *)info->mData + offset,
 &numPageSamples,
 sizeof(numPageSamples));

            offset += sizeof(numPageSamples);
 }

 if (releaseBuffer) {
            srcBuffer->release();
            srcBuffer = NULL;
 }

 ++n;

 if (!(mQuirks & kSupportsMultipleFramesPerInputBuffer)) {
 break;
 }

 int64_t coalescedDurationUs = lastBufferTimeUs - timestampUs;

 if (coalescedDurationUs > 250000ll) {
 break;
 }
 }

 if (n > 1) {
        ALOGV("coalesced %d frames into one input buffer", n);
 }

    OMX_U32 flags = OMX_BUFFERFLAG_ENDOFFRAME;

 if (signalEOS) {
        flags |= OMX_BUFFERFLAG_EOS;
 } else {
        mNoMoreOutputData = false;
 }

 if (info == NULL) {
        CHECK(mFlags & kUseSecureInputBuffers);
        CHECK(signalEOS);

        info = findEmptyInputBuffer();
 }

    CODEC_LOGV("Calling emptyBuffer on buffer %u (length %zu), "
 "timestamp %lld us (%.2f secs)",
               info->mBuffer, offset,
 (long long)timestampUs, timestampUs / 1E6);

    err = mOMX->emptyBuffer(
            mNode, info->mBuffer, 0, offset,
            flags, timestampUs);

 if (err != OK) {
        setState(ERROR);
 return false;
 }

    info->mStatus = OWNED_BY_COMPONENT;

 return true;
}

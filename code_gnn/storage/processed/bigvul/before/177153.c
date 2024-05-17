FLAC__StreamEncoderWriteStatus SoftFlacEncoder::onEncodedFlacAvailable(
 const FLAC__byte buffer[],
 size_t bytes, unsigned samples,
 unsigned current_frame) {
    UNUSED_UNLESS_VERBOSE(current_frame);
    ALOGV("SoftFlacEncoder::onEncodedFlacAvailable(bytes=%zu, samples=%u, curr_frame=%u)",
            bytes, samples, current_frame);

#ifdef WRITE_FLAC_HEADER_IN_FIRST_BUFFER
 if (samples == 0) {
        ALOGI(" saving %zu bytes of header", bytes);
        memcpy(mHeader + mHeaderOffset, buffer, bytes);
        mHeaderOffset += bytes; 
 return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
 }

#endif

 if ((samples == 0) || !mEncoderWriteData) {
        ALOGV("ignoring %zu bytes of header data (samples=%d)", bytes, samples);
 return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
 }

 List<BufferInfo *> &outQueue = getPortQueue(1);
    CHECK(!outQueue.empty());
 BufferInfo *outInfo = *outQueue.begin();
    OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;

#ifdef WRITE_FLAC_HEADER_IN_FIRST_BUFFER
 if (!mWroteHeader) {
        ALOGI(" writing %d bytes of header on output port", mHeaderOffset);
        memcpy(outHeader->pBuffer + outHeader->nOffset + outHeader->nFilledLen,
                mHeader, mHeaderOffset);
        outHeader->nFilledLen += mHeaderOffset;
        outHeader->nOffset    += mHeaderOffset;
        mWroteHeader = true;
 }
#endif

    ALOGV(" writing %zu bytes of encoded data on output port", bytes);
 if (bytes > outHeader->nAllocLen - outHeader->nOffset - outHeader->nFilledLen) {
        ALOGE(" not enough space left to write encoded data, dropping %zu bytes", bytes);
 return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
 }
    memcpy(outHeader->pBuffer + outHeader->nOffset, buffer, bytes);

    outHeader->nTimeStamp = mCurrentInputTimeStamp;
    outHeader->nOffset = 0;
    outHeader->nFilledLen += bytes;
    outHeader->nFlags = 0;

    mEncoderReturnedEncodedData = true;
    mEncoderReturnedNbBytes += bytes;

 return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

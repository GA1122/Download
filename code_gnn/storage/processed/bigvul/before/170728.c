status_t OMXNodeInstance::fillBuffer(OMX::buffer_id buffer) {
 Mutex::Autolock autoLock(mLock);

    OMX_BUFFERHEADERTYPE *header = findBufferHeader(buffer);
    header->nFilledLen = 0;
    header->nOffset = 0;
    header->nFlags = 0;

 {
 Mutex::Autolock _l(mDebugLock);
        mOutputBuffersWithCodec.add(header);
        CLOG_BUMPED_BUFFER(fillBuffer, WITH_STATS(EMPTY_BUFFER(buffer, header)));
 }

    OMX_ERRORTYPE err = OMX_FillThisBuffer(mHandle, header);
 if (err != OMX_ErrorNone) {
        CLOG_ERROR(fillBuffer, err, EMPTY_BUFFER(buffer, header));
 Mutex::Autolock _l(mDebugLock);
        mOutputBuffersWithCodec.remove(header);
 }
 return StatusFromOMXError(err);
}
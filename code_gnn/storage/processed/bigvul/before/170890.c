status_t OMXNodeInstance::emptyBuffer_l(
        OMX_BUFFERHEADERTYPE *header, OMX_U32 flags, OMX_TICKS timestamp,
 intptr_t debugAddr, int fenceFd) {
    header->nFlags = flags;
    header->nTimeStamp = timestamp;

 status_t res = storeFenceInMeta_l(header, fenceFd, kPortIndexInput);
 if (res != OK) {
        CLOG_ERROR(emptyBuffer::storeFenceInMeta, res, WITH_STATS(
                FULL_BUFFER(debugAddr, header, fenceFd)));
 return res;
 }

 {
 Mutex::Autolock _l(mDebugLock);
        mInputBuffersWithCodec.add(header);

 if ((flags & OMX_BUFFERFLAG_CODECCONFIG) != 0) {
            bumpDebugLevel_l(2  , 0  );
 }

        CLOG_BUMPED_BUFFER(emptyBuffer, WITH_STATS(FULL_BUFFER(debugAddr, header, fenceFd)));
 }

    OMX_ERRORTYPE err = OMX_EmptyThisBuffer(mHandle, header);
    CLOG_IF_ERROR(emptyBuffer, err, FULL_BUFFER(debugAddr, header, fenceFd));

 {
 Mutex::Autolock _l(mDebugLock);
 if (err != OMX_ErrorNone) {
            mInputBuffersWithCodec.remove(header);
 } else if (!(flags & OMX_BUFFERFLAG_CODECCONFIG)) {
            unbumpDebugLevel_l(kPortIndexInput);
 }
 }

 return StatusFromOMXError(err);
}

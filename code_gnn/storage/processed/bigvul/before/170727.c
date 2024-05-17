status_t OMXNodeInstance::emptyDirectBuffer(
        OMX_BUFFERHEADERTYPE *header,
        OMX_U32 rangeOffset, OMX_U32 rangeLength,
        OMX_U32 flags, OMX_TICKS timestamp) {
 Mutex::Autolock autoLock(mLock);

    header->nFilledLen = rangeLength;
    header->nOffset = rangeOffset;

 return emptyBuffer_l(header, flags, timestamp, (intptr_t)header->pBuffer);
}

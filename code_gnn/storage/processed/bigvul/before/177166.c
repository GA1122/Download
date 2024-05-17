void SoftRaw::onQueueFilled(OMX_U32  ) {
 if (mSignalledError) {
 return;
 }

 List<BufferInfo *> &inQueue = getPortQueue(0);
 List<BufferInfo *> &outQueue = getPortQueue(1);

 while (!inQueue.empty() && !outQueue.empty()) {
 BufferInfo *inInfo = *inQueue.begin();
        OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;

 BufferInfo *outInfo = *outQueue.begin();
        OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;

        CHECK_GE(outHeader->nAllocLen, inHeader->nFilledLen);
        memcpy(outHeader->pBuffer,
               inHeader->pBuffer + inHeader->nOffset,
               inHeader->nFilledLen);

        outHeader->nFlags = inHeader->nFlags;
        outHeader->nOffset = 0;
        outHeader->nFilledLen = inHeader->nFilledLen;
        outHeader->nTimeStamp = inHeader->nTimeStamp;

 bool sawEOS = (inHeader->nFlags & OMX_BUFFERFLAG_EOS) != 0;

        inQueue.erase(inQueue.begin());
        inInfo->mOwnedByUs = false;
        notifyEmptyBufferDone(inHeader);

        outQueue.erase(outQueue.begin());
        outInfo->mOwnedByUs = false;
        notifyFillBufferDone(outHeader);

 if (sawEOS) {
 break;
 }
 }
}

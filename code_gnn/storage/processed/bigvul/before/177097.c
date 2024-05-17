void SoftAMRNBEncoder::onQueueFilled(OMX_U32  ) {
 if (mSignalledError) {
 return;
 }

 List<BufferInfo *> &inQueue = getPortQueue(0);
 List<BufferInfo *> &outQueue = getPortQueue(1);

 size_t numBytesPerInputFrame = kNumSamplesPerFrame * sizeof(int16_t);

 for (;;) {

 while (mInputSize < numBytesPerInputFrame) {

 if (mSawInputEOS || inQueue.empty()) {
 return;
 }

 BufferInfo *inInfo = *inQueue.begin();
            OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;

 const void *inData = inHeader->pBuffer + inHeader->nOffset;

 size_t copy = numBytesPerInputFrame - mInputSize;
 if (copy > inHeader->nFilledLen) {
                copy = inHeader->nFilledLen;
 }

 if (mInputSize == 0) {
                mInputTimeUs = inHeader->nTimeStamp;
 }

            memcpy((uint8_t *)mInputFrame + mInputSize, inData, copy);
            mInputSize += copy;

            inHeader->nOffset += copy;
            inHeader->nFilledLen -= copy;

            inHeader->nTimeStamp +=
 (copy * 1000000ll / kSampleRate) / sizeof(int16_t);

 if (inHeader->nFilledLen == 0) {
 if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
                    ALOGV("saw input EOS");
                    mSawInputEOS = true;

                    memset((uint8_t *)mInputFrame + mInputSize,
 0,
                           numBytesPerInputFrame - mInputSize);

                    mInputSize = numBytesPerInputFrame;
 }

                inQueue.erase(inQueue.begin());
                inInfo->mOwnedByUs = false;
                notifyEmptyBufferDone(inHeader);

                inData = NULL;
                inHeader = NULL;
                inInfo = NULL;
 }
 }


 if (outQueue.empty()) {
 return;
 }

 BufferInfo *outInfo = *outQueue.begin();
        OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;

 uint8_t *outPtr = outHeader->pBuffer + outHeader->nOffset;
 size_t outAvailable = outHeader->nAllocLen - outHeader->nOffset;

 Frame_Type_3GPP frameType;
 int res = AMREncode(
                mEncState, mSidState, (Mode)mMode,
                mInputFrame, outPtr, &frameType, AMR_TX_WMF);

        CHECK_GE(res, 0);
        CHECK_LE((size_t)res, outAvailable);

        outPtr[0] = ((outPtr[0] << 3) | 4) & 0x7c;

        outHeader->nFilledLen = res;
        outHeader->nFlags = OMX_BUFFERFLAG_ENDOFFRAME;

 if (mSawInputEOS) {
            outHeader->nFlags = OMX_BUFFERFLAG_EOS;
 }

        outHeader->nTimeStamp = mInputTimeUs;

#if 0
        ALOGI("sending %d bytes of data (time = %lld us, flags = 0x%08lx)",
              nOutputBytes, mInputTimeUs, outHeader->nFlags);

        hexdump(outHeader->pBuffer + outHeader->nOffset, outHeader->nFilledLen);
#endif

        outQueue.erase(outQueue.begin());
        outInfo->mOwnedByUs = false;
        notifyFillBufferDone(outHeader);

        outHeader = NULL;
        outInfo = NULL;

        mInputSize = 0;
 }
}

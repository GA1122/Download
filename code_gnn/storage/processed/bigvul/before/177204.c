status_t ACodec::allocateOutputMetadataBuffers() {
    OMX_U32 bufferCount, bufferSize, minUndequeuedBuffers;
 status_t err = configureOutputBuffersFromNativeWindow(
 &bufferCount, &bufferSize, &minUndequeuedBuffers);
 if (err != 0)
 return err;
    mNumUndequeuedBuffers = minUndequeuedBuffers;

    ALOGV("[%s] Allocating %u meta buffers on output port",
         mComponentName.c_str(), bufferCount);

 size_t bufSize = mOutputMetadataType == kMetadataBufferTypeANWBuffer ?
 sizeof(struct VideoNativeMetadata) : sizeof(struct VideoGrallocMetadata);
 size_t totalSize = bufferCount * bufSize;
    mDealer[kPortIndexOutput] = new MemoryDealer(totalSize, "ACodec");

 for (OMX_U32 i = 0; i < bufferCount; i++) {
 BufferInfo info;
        info.mStatus = BufferInfo::OWNED_BY_NATIVE_WINDOW;
        info.mFenceFd = -1;
        info.mRenderInfo = NULL;
        info.mGraphicBuffer = NULL;
        info.mDequeuedAt = mDequeueCounter;

        sp<IMemory> mem = mDealer[kPortIndexOutput]->allocate(bufSize);
 if (mem == NULL || mem->pointer() == NULL) {
 return NO_MEMORY;
 }
 if (mOutputMetadataType == kMetadataBufferTypeANWBuffer) {
 ((VideoNativeMetadata *)mem->pointer())->nFenceFd = -1;
 }
        info.mData = new ABuffer(mem->pointer(), mem->size());

        err = mOMX->useBuffer(
                mNode, kPortIndexOutput, mem, &info.mBufferID, mem->size());

        mBuffers[kPortIndexOutput].push(info);

        ALOGV("[%s] allocated meta buffer with ID %u (pointer = %p)",
             mComponentName.c_str(), info.mBufferID, mem->pointer());
 }

 if (mLegacyAdaptiveExperiment) {
 static_cast<Surface *>(mNativeWindow.get())
 ->getIGraphicBufferProducer()->allowAllocation(true);

        ALOGV("[%s] Allocating %u buffers from a native window of size %u on "
 "output port",
             mComponentName.c_str(), bufferCount, bufferSize);

 for (OMX_U32 i = 0; i < bufferCount; i++) {
 BufferInfo *info = &mBuffers[kPortIndexOutput].editItemAt(i);

 ANativeWindowBuffer *buf;
 int fenceFd;
            err = mNativeWindow->dequeueBuffer(mNativeWindow.get(), &buf, &fenceFd);
 if (err != 0) {
                ALOGE("dequeueBuffer failed: %s (%d)", strerror(-err), -err);
 break;
 }

            sp<GraphicBuffer> graphicBuffer(new GraphicBuffer(buf, false));
            mOMX->updateGraphicBufferInMeta(
                    mNode, kPortIndexOutput, graphicBuffer, info->mBufferID);
            info->mStatus = BufferInfo::OWNED_BY_US;
            info->setWriteFence(fenceFd, "allocateOutputMetadataBuffers for legacy");
            info->mGraphicBuffer = graphicBuffer;
 }

 for (OMX_U32 i = 0; i < mBuffers[kPortIndexOutput].size(); i++) {
 BufferInfo *info = &mBuffers[kPortIndexOutput].editItemAt(i);
 if (info->mStatus == BufferInfo::OWNED_BY_US) {
 status_t error = cancelBufferToNativeWindow(info);
 if (err == OK) {
                    err = error;
 }
 }
 }

 static_cast<Surface*>(mNativeWindow.get())
 ->getIGraphicBufferProducer()->allowAllocation(false);
 }

    mMetadataBuffersToSubmit = bufferCount - minUndequeuedBuffers;
 return err;
}

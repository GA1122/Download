ACodec::BufferInfo *ACodec::dequeueBufferFromNativeWindow() {
 ANativeWindowBuffer *buf;
    CHECK(mNativeWindow.get() != NULL);

 if (mTunneled) {
        ALOGW("dequeueBufferFromNativeWindow() should not be called in tunnel"
 " video playback mode mode!");
 return NULL;
 }

 if (mFatalError) {
        ALOGW("not dequeuing from native window due to fatal error");
 return NULL;
 }

 int fenceFd = -1;
 do {
 status_t err = mNativeWindow->dequeueBuffer(mNativeWindow.get(), &buf, &fenceFd);
 if (err != 0) {
            ALOGE("dequeueBuffer failed: %s(%d).", asString(err), err);
 return NULL;
 }

 bool stale = false;
 for (size_t i = mBuffers[kPortIndexOutput].size(); i-- > 0;) {
 BufferInfo *info = &mBuffers[kPortIndexOutput].editItemAt(i);

 if (info->mGraphicBuffer != NULL &&
                    info->mGraphicBuffer->handle == buf->handle) {
 if (info->mStatus != BufferInfo::OWNED_BY_NATIVE_WINDOW) {
                    ALOGI("dequeued stale buffer %p. discarding", buf);
                    stale = true;
 break;
 }

                ALOGV("dequeued buffer %p", info->mGraphicBuffer->getNativeBuffer());
                info->mStatus = BufferInfo::OWNED_BY_US;
                info->setWriteFence(fenceFd, "dequeueBufferFromNativeWindow");
                updateRenderInfoForDequeuedBuffer(buf, fenceFd, info);
 return info;
 }
 }

 if (!stale && (!storingMetadataInDecodedBuffers() || mLegacyAdaptiveExperiment)) {
            ALOGI("dequeued unrecognized (stale) buffer %p. discarding", buf);
            stale = true;
 }
 if (stale) {
            buf = NULL;
 }
 } while (buf == NULL);

 BufferInfo *oldest = NULL;
 for (size_t i = mBuffers[kPortIndexOutput].size(); i-- > 0;) {
 BufferInfo *info =
 &mBuffers[kPortIndexOutput].editItemAt(i);
 if (info->mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW &&
 (oldest == NULL ||
             mDequeueCounter - info->mDequeuedAt >
                    mDequeueCounter - oldest->mDequeuedAt)) {
            oldest = info;
 }
 }

    CHECK(oldest != NULL);
    CHECK(storingMetadataInDecodedBuffers());

    oldest->mGraphicBuffer = new GraphicBuffer(buf, false);
    oldest->mStatus = BufferInfo::OWNED_BY_US;
    oldest->setWriteFence(fenceFd, "dequeueBufferFromNativeWindow for oldest");
    mRenderTracker.untrackFrame(oldest->mRenderInfo);
    oldest->mRenderInfo = NULL;

    mOMX->updateGraphicBufferInMeta(
            mNode, kPortIndexOutput, oldest->mGraphicBuffer,
            oldest->mBufferID);

 if (mOutputMetadataType == kMetadataBufferTypeGrallocSource) {
 VideoGrallocMetadata *grallocMeta =
 reinterpret_cast<VideoGrallocMetadata *>(oldest->mData->base());
        ALOGV("replaced oldest buffer #%u with age %u (%p/%p stored in %p)",
 (unsigned)(oldest - &mBuffers[kPortIndexOutput][0]),
                mDequeueCounter - oldest->mDequeuedAt,
 (void *)(uintptr_t)grallocMeta->pHandle,
                oldest->mGraphicBuffer->handle, oldest->mData->base());
 } else if (mOutputMetadataType == kMetadataBufferTypeANWBuffer) {
 VideoNativeMetadata *nativeMeta =
 reinterpret_cast<VideoNativeMetadata *>(oldest->mData->base());
        ALOGV("replaced oldest buffer #%u with age %u (%p/%p stored in %p)",
 (unsigned)(oldest - &mBuffers[kPortIndexOutput][0]),
                mDequeueCounter - oldest->mDequeuedAt,
 (void *)(uintptr_t)nativeMeta->pBuffer,
                oldest->mGraphicBuffer->getNativeBuffer(), oldest->mData->base());
 }

    updateRenderInfoForDequeuedBuffer(buf, fenceFd, oldest);
 return oldest;
}

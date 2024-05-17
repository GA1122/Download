status_t ACodec::handleSetSurface(const sp<Surface> &surface) {
 if (surface == NULL) {
 if (mNativeWindow != NULL) {
            ALOGW("cannot unset a surface");
 return INVALID_OPERATION;
 }
 return OK;
 }

 if (mNativeWindow == NULL) {
        ALOGW("component was not configured with a surface");
 return INVALID_OPERATION;
 }

 ANativeWindow *nativeWindow = surface.get();
 if (mBuffers[kPortIndexInput].size() == 0) {
        mNativeWindow = surface;
 return OK;
 }

 if (mTunneled) {
        ALOGW("cannot change tunneled surface");
 return INVALID_OPERATION;
 }

 int usageBits = 0;
 status_t err = setupNativeWindowSizeFormatAndUsage(nativeWindow, &usageBits);
 if (err != OK) {
 return err;
 }

 int ignoredFlags = kVideoGrallocUsage;
 if ((usageBits & ~(mNativeWindowUsageBits | ignoredFlags)) != 0) {
        ALOGW("cannot change usage from %#x to %#x", mNativeWindowUsageBits, usageBits);
 return BAD_VALUE;
 }

 int minUndequeuedBuffers = 0;
    err = nativeWindow->query(
            nativeWindow, NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
 &minUndequeuedBuffers);
 if (err != 0) {
        ALOGE("NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS query failed: %s (%d)",
                strerror(-err), -err);
 return err;
 }
 if (minUndequeuedBuffers > (int)mNumUndequeuedBuffers) {
        ALOGE("new surface holds onto more buffers (%d) than planned for (%zu)",
                minUndequeuedBuffers, mNumUndequeuedBuffers);
 return BAD_VALUE;
 }

 Vector<BufferInfo> &buffers = mBuffers[kPortIndexOutput];
    ALOGV("setting up surface for %zu buffers", buffers.size());

    err = native_window_set_buffer_count(nativeWindow, buffers.size());
 if (err != 0) {
        ALOGE("native_window_set_buffer_count failed: %s (%d)", strerror(-err),
 -err);
 return err;
 }

    surface->getIGraphicBufferProducer()->allowAllocation(true);

 for (size_t i = 0; i < buffers.size(); ++i) {
 const BufferInfo &info = buffers[i];
 if (storingMetadataInDecodedBuffers()
 && !mLegacyAdaptiveExperiment
 && info.mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW) {
            ALOGV("skipping buffer %p", info.mGraphicBuffer->getNativeBuffer());
 continue;
 }
        ALOGV("attaching buffer %p", info.mGraphicBuffer->getNativeBuffer());

        err = surface->attachBuffer(info.mGraphicBuffer->getNativeBuffer());
 if (err != OK) {
            ALOGE("failed to attach buffer %p to the new surface: %s (%d)",
                    info.mGraphicBuffer->getNativeBuffer(),
                    strerror(-err), -err);
 return err;
 }
 }

 if (!storingMetadataInDecodedBuffers() || mLegacyAdaptiveExperiment) {
 for (size_t i = 0; i < buffers.size(); ++i) {
 BufferInfo &info = buffers.editItemAt(i);
 if (info.mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW) {
                ALOGV("canceling buffer %p", info.mGraphicBuffer->getNativeBuffer());
                err = nativeWindow->cancelBuffer(
                        nativeWindow, info.mGraphicBuffer->getNativeBuffer(), info.mFenceFd);
                info.mFenceFd = -1;
 if (err != OK) {
                    ALOGE("failed to cancel buffer %p to the new surface: %s (%d)",
                            info.mGraphicBuffer->getNativeBuffer(),
                            strerror(-err), -err);
 return err;
 }
 }
 }
 (void)surface->getIGraphicBufferProducer()->allowAllocation(false);
 }

 if (mFlags & kFlagPushBlankBuffersToNativeWindowOnShutdown) {
        pushBlankBuffersToNativeWindow(mNativeWindow.get());
 }

    mNativeWindow = nativeWindow;
    mNativeWindowUsageBits = usageBits;
 return OK;
}

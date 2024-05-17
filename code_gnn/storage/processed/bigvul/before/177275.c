bool ACodec::BaseState::onOMXFillBufferDone(
        IOMX::buffer_id bufferID,
 size_t rangeOffset, size_t rangeLength,
        OMX_U32 flags,
 int64_t timeUs,
 int fenceFd) {
    ALOGV("[%s] onOMXFillBufferDone %u time %" PRId64 " us, flags = 0x%08x",
         mCodec->mComponentName.c_str(), bufferID, timeUs, flags);

 ssize_t index;
 status_t err= OK;

#if TRACK_BUFFER_TIMING
    index = mCodec->mBufferStats.indexOfKey(timeUs);
 if (index >= 0) {
 ACodec::BufferStats *stats = &mCodec->mBufferStats.editValueAt(index);
        stats->mFillBufferDoneTimeUs = ALooper::GetNowUs();

        ALOGI("frame PTS %lld: %lld",
                timeUs,
                stats->mFillBufferDoneTimeUs - stats->mEmptyBufferTimeUs);

        mCodec->mBufferStats.removeItemsAt(index);
        stats = NULL;
 }
#endif

 BufferInfo *info =
        mCodec->findBufferByID(kPortIndexOutput, bufferID, &index);
 BufferInfo::Status status = BufferInfo::getSafeStatus(info);
 if (status != BufferInfo::OWNED_BY_COMPONENT) {
        ALOGE("Wrong ownership in FBD: %s(%d) buffer #%u", _asString(status), status, bufferID);
        mCodec->dumpBuffers(kPortIndexOutput);
        mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 if (fenceFd >= 0) {
 ::close(fenceFd);
 }
 return true;
 }

    info->mDequeuedAt = ++mCodec->mDequeueCounter;
    info->mStatus = BufferInfo::OWNED_BY_US;

 if (info->mRenderInfo != NULL) {
        mCodec->notifyOfRenderedFrames(true  );
 }

 if (mCodec->mNativeWindow == NULL) {
 (void)mCodec->waitForFence(fenceFd, "onOMXFillBufferDone");
        fenceFd = -1;
 }
    info->setReadFence(fenceFd, "onOMXFillBufferDone");

 PortMode mode = getPortMode(kPortIndexOutput);

 switch (mode) {
 case KEEP_BUFFERS:
 break;

 case RESUBMIT_BUFFERS:
 {
 if (rangeLength == 0 && (!(flags & OMX_BUFFERFLAG_EOS)
 || mCodec->mPortEOS[kPortIndexOutput])) {
                ALOGV("[%s] calling fillBuffer %u",
                     mCodec->mComponentName.c_str(), info->mBufferID);

                err = mCodec->mOMX->fillBuffer(mCodec->mNode, info->mBufferID, info->mFenceFd);
                info->mFenceFd = -1;
 if (err != OK) {
                    mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 return true;
 }

                info->mStatus = BufferInfo::OWNED_BY_COMPONENT;
 break;
 }

            sp<AMessage> reply =
 new AMessage(kWhatOutputBufferDrained, mCodec);

 if (!mCodec->mSentFormat && rangeLength > 0) {
                mCodec->sendFormatChange(reply);
 }
 if (mCodec->usingMetadataOnEncoderOutput()) {
 native_handle_t *handle = NULL;
 VideoGrallocMetadata &grallocMeta = *(VideoGrallocMetadata *)info->mData->data();
 VideoNativeMetadata &nativeMeta = *(VideoNativeMetadata *)info->mData->data();
 if (info->mData->size() >= sizeof(grallocMeta)
 && grallocMeta.eType == kMetadataBufferTypeGrallocSource) {
                    handle = (native_handle_t *)(uintptr_t)grallocMeta.pHandle;
 } else if (info->mData->size() >= sizeof(nativeMeta)
 && nativeMeta.eType == kMetadataBufferTypeANWBuffer) {
#ifdef OMX_ANDROID_COMPILE_AS_32BIT_ON_64BIT_PLATFORMS
                    handle = NULL;
#else
                    handle = (native_handle_t *)nativeMeta.pBuffer->handle;
#endif
 }
                info->mData->meta()->setPointer("handle", handle);
                info->mData->meta()->setInt32("rangeOffset", rangeOffset);
                info->mData->meta()->setInt32("rangeLength", rangeLength);
 } else {
                info->mData->setRange(rangeOffset, rangeLength);
 }
#if 0
 if (mCodec->mNativeWindow == NULL) {
 if (IsIDR(info->mData)) {
                    ALOGI("IDR frame");
 }
 }
#endif

 if (mCodec->mSkipCutBuffer != NULL) {
                mCodec->mSkipCutBuffer->submit(info->mData);
 }
            info->mData->meta()->setInt64("timeUs", timeUs);

            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatDrainThisBuffer);
            notify->setInt32("buffer-id", info->mBufferID);
            notify->setBuffer("buffer", info->mData);
            notify->setInt32("flags", flags);

            reply->setInt32("buffer-id", info->mBufferID);

            notify->setMessage("reply", reply);

            notify->post();

            info->mStatus = BufferInfo::OWNED_BY_DOWNSTREAM;

 if (flags & OMX_BUFFERFLAG_EOS) {
                ALOGV("[%s] saw output EOS", mCodec->mComponentName.c_str());

                sp<AMessage> notify = mCodec->mNotify->dup();
                notify->setInt32("what", CodecBase::kWhatEOS);
                notify->setInt32("err", mCodec->mInputEOSResult);
                notify->post();

                mCodec->mPortEOS[kPortIndexOutput] = true;
 }
 break;
 }

 case FREE_BUFFERS:
            err = mCodec->freeBuffer(kPortIndexOutput, index);
 if (err != OK) {
                mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 return true;
 }
 break;

 default:
            ALOGE("Invalid port mode: %d", mode);
 return false;
 }

 return true;
}

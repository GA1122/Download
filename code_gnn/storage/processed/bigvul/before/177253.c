void ACodec::BaseState::onInputBufferFilled(const sp<AMessage> &msg) {
    IOMX::buffer_id bufferID;
    CHECK(msg->findInt32("buffer-id", (int32_t*)&bufferID));
    sp<ABuffer> buffer;
 int32_t err = OK;
 bool eos = false;
 PortMode mode = getPortMode(kPortIndexInput);

 if (!msg->findBuffer("buffer", &buffer)) {
  
        CHECK(msg->findInt32("err", &err));

 if (err == OK) {
  
            mode = KEEP_BUFFERS;
 } else {
            ALOGV("[%s] saw error %d instead of an input buffer",
                 mCodec->mComponentName.c_str(), err);
            eos = true;
 }

        buffer.clear();
 }

 int32_t tmp;
 if (buffer != NULL && buffer->meta()->findInt32("eos", &tmp) && tmp) {
        eos = true;
        err = ERROR_END_OF_STREAM;
 }

 BufferInfo *info = mCodec->findBufferByID(kPortIndexInput, bufferID);
 BufferInfo::Status status = BufferInfo::getSafeStatus(info);
 if (status != BufferInfo::OWNED_BY_UPSTREAM) {
        ALOGE("Wrong ownership in IBF: %s(%d) buffer #%u", _asString(status), status, bufferID);
        mCodec->dumpBuffers(kPortIndexInput);
        mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return;
 }

    info->mStatus = BufferInfo::OWNED_BY_US;

 switch (mode) {
 case KEEP_BUFFERS:
 {
 if (eos) {
 if (!mCodec->mPortEOS[kPortIndexInput]) {
                    mCodec->mPortEOS[kPortIndexInput] = true;
                    mCodec->mInputEOSResult = err;
 }
 }
 break;
 }

 case RESUBMIT_BUFFERS:
 {
 if (buffer != NULL && !mCodec->mPortEOS[kPortIndexInput]) {
 if (buffer->size() == 0 && !eos) {
                    postFillThisBuffer(info);
 break;
 }

 int64_t timeUs;
                CHECK(buffer->meta()->findInt64("timeUs", &timeUs));

                OMX_U32 flags = OMX_BUFFERFLAG_ENDOFFRAME;

 int32_t isCSD;
 if (buffer->meta()->findInt32("csd", &isCSD) && isCSD != 0) {
                    flags |= OMX_BUFFERFLAG_CODECCONFIG;
 }

 if (eos) {
                    flags |= OMX_BUFFERFLAG_EOS;
 }

 if (buffer != info->mData) {
                    ALOGV("[%s] Needs to copy input data for buffer %u. (%p != %p)",
                         mCodec->mComponentName.c_str(),
                         bufferID,
                         buffer.get(), info->mData.get());

 if (buffer->size() > info->mData->capacity()) {
                        ALOGE("data size (%zu) is greated than buffer capacity (%zu)",
                                buffer->size(),  
                                info->mData->capacity());  
                        mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return;
 }
                    memcpy(info->mData->data(), buffer->data(), buffer->size());
 }

 if (flags & OMX_BUFFERFLAG_CODECCONFIG) {
                    ALOGV("[%s] calling emptyBuffer %u w/ codec specific data",
                         mCodec->mComponentName.c_str(), bufferID);
 } else if (flags & OMX_BUFFERFLAG_EOS) {
                    ALOGV("[%s] calling emptyBuffer %u w/ EOS",
                         mCodec->mComponentName.c_str(), bufferID);
 } else {
#if TRACK_BUFFER_TIMING
                    ALOGI("[%s] calling emptyBuffer %u w/ time %lld us",
                         mCodec->mComponentName.c_str(), bufferID, (long long)timeUs);
#else
                    ALOGV("[%s] calling emptyBuffer %u w/ time %lld us",
                         mCodec->mComponentName.c_str(), bufferID, (long long)timeUs);
#endif
 }

#if TRACK_BUFFER_TIMING
 ACodec::BufferStats stats;
                stats.mEmptyBufferTimeUs = ALooper::GetNowUs();
                stats.mFillBufferDoneTimeUs = -1ll;
                mCodec->mBufferStats.add(timeUs, stats);
#endif

 if (mCodec->storingMetadataInDecodedBuffers()) {
 PortMode outputMode = getPortMode(kPortIndexOutput);

                    ALOGV("MetadataBuffersToSubmit=%u portMode=%s",
                            mCodec->mMetadataBuffersToSubmit,
 (outputMode == FREE_BUFFERS ? "FREE" :
                             outputMode == KEEP_BUFFERS ? "KEEP" : "RESUBMIT"));
 if (outputMode == RESUBMIT_BUFFERS) {
                        mCodec->submitOutputMetadataBuffer();
 }
 }
                info->checkReadFence("onInputBufferFilled");
 status_t err2 = mCodec->mOMX->emptyBuffer(
                    mCodec->mNode,
                    bufferID,
 0,
                    buffer->size(),
                    flags,
                    timeUs,
                    info->mFenceFd);
                info->mFenceFd = -1;
 if (err2 != OK) {
                    mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err2));
 return;
 }
                info->mStatus = BufferInfo::OWNED_BY_COMPONENT;

 if (!eos && err == OK) {
                    getMoreInputDataIfPossible();
 } else {
                    ALOGV("[%s] Signalled EOS (%d) on the input port",
                         mCodec->mComponentName.c_str(), err);

                    mCodec->mPortEOS[kPortIndexInput] = true;
                    mCodec->mInputEOSResult = err;
 }
 } else if (!mCodec->mPortEOS[kPortIndexInput]) {
 if (err != OK && err != ERROR_END_OF_STREAM) {
                    ALOGV("[%s] Signalling EOS on the input port due to error %d",
                         mCodec->mComponentName.c_str(), err);
 } else {
                    ALOGV("[%s] Signalling EOS on the input port",
                         mCodec->mComponentName.c_str());
 }

                ALOGV("[%s] calling emptyBuffer %u signalling EOS",
                     mCodec->mComponentName.c_str(), bufferID);

                info->checkReadFence("onInputBufferFilled");
 status_t err2 = mCodec->mOMX->emptyBuffer(
                        mCodec->mNode,
                        bufferID,
 0,
 0,
                        OMX_BUFFERFLAG_EOS,
 0,
                        info->mFenceFd);
                info->mFenceFd = -1;
 if (err2 != OK) {
                    mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err2));
 return;
 }
                info->mStatus = BufferInfo::OWNED_BY_COMPONENT;

                mCodec->mPortEOS[kPortIndexInput] = true;
                mCodec->mInputEOSResult = err;
 }
 break;
 }

 case FREE_BUFFERS:
 break;

 default:
            ALOGE("invalid port mode: %d", mode);
 break;
 }
}

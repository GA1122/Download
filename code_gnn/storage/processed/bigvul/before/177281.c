void ACodec::BaseState::onOutputBufferDrained(const sp<AMessage> &msg) {
    IOMX::buffer_id bufferID;
    CHECK(msg->findInt32("buffer-id", (int32_t*)&bufferID));
 ssize_t index;
 BufferInfo *info = mCodec->findBufferByID(kPortIndexOutput, bufferID, &index);
 BufferInfo::Status status = BufferInfo::getSafeStatus(info);
 if (status != BufferInfo::OWNED_BY_DOWNSTREAM) {
        ALOGE("Wrong ownership in OBD: %s(%d) buffer #%u", _asString(status), status, bufferID);
        mCodec->dumpBuffers(kPortIndexOutput);
        mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return;
 }

 android_native_rect_t crop;
 if (msg->findRect("crop", &crop.left, &crop.top, &crop.right, &crop.bottom)) {
 status_t err = native_window_set_crop(mCodec->mNativeWindow.get(), &crop);
        ALOGW_IF(err != NO_ERROR, "failed to set crop: %d", err);
 }

 int32_t render;
 if (mCodec->mNativeWindow != NULL
 && msg->findInt32("render", &render) && render != 0
 && info->mData != NULL && info->mData->size() != 0) {
        ATRACE_NAME("render");

 int64_t mediaTimeUs = -1;
        info->mData->meta()->findInt64("timeUs", &mediaTimeUs);
 if (mediaTimeUs >= 0) {
            mCodec->mRenderTracker.onFrameQueued(
                    mediaTimeUs, info->mGraphicBuffer, new Fence(::dup(info->mFenceFd)));
 }

 int64_t timestampNs = 0;
 if (!msg->findInt64("timestampNs", &timestampNs)) {
 if (info->mData->meta()->findInt64("timeUs", &timestampNs)) {
                ALOGV("using buffer PTS of %lld", (long long)timestampNs);
                timestampNs *= 1000;
 }
 }

 status_t err;
        err = native_window_set_buffers_timestamp(mCodec->mNativeWindow.get(), timestampNs);
        ALOGW_IF(err != NO_ERROR, "failed to set buffer timestamp: %d", err);

        info->checkReadFence("onOutputBufferDrained before queueBuffer");
        err = mCodec->mNativeWindow->queueBuffer(
                    mCodec->mNativeWindow.get(), info->mGraphicBuffer.get(), info->mFenceFd);
        info->mFenceFd = -1;
 if (err == OK) {
            info->mStatus = BufferInfo::OWNED_BY_NATIVE_WINDOW;
 } else {
            ALOGE("queueBuffer failed in onOutputBufferDrained: %d", err);
            mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
            info->mStatus = BufferInfo::OWNED_BY_US;
            info->mIsReadFence = false;
 }
 } else {
 if (mCodec->mNativeWindow != NULL &&
 (info->mData == NULL || info->mData->size() != 0)) {
            info->mIsReadFence = false;
            ATRACE_NAME("frame-drop");
 }
        info->mStatus = BufferInfo::OWNED_BY_US;
 }

 PortMode mode = getPortMode(kPortIndexOutput);

 switch (mode) {
 case KEEP_BUFFERS:
 {

 if (info->mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW) {

                info = mCodec->dequeueBufferFromNativeWindow();
 }
 break;
 }

 case RESUBMIT_BUFFERS:
 {
 if (!mCodec->mPortEOS[kPortIndexOutput]) {
 if (info->mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW) {

                    info = mCodec->dequeueBufferFromNativeWindow();
 }

 if (info != NULL) {
                    ALOGV("[%s] calling fillBuffer %u",
                         mCodec->mComponentName.c_str(), info->mBufferID);
                    info->checkWriteFence("onOutputBufferDrained::RESUBMIT_BUFFERS");
 status_t err = mCodec->mOMX->fillBuffer(
                            mCodec->mNode, info->mBufferID, info->mFenceFd);
                    info->mFenceFd = -1;
 if (err == OK) {
                        info->mStatus = BufferInfo::OWNED_BY_COMPONENT;
 } else {
                        mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 }
 }
 }
 break;
 }

 case FREE_BUFFERS:
 {
 status_t err = mCodec->freeBuffer(kPortIndexOutput, index);
 if (err != OK) {
                mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 }
 break;
 }

 default:
            ALOGE("Invalid port mode: %d", mode);
 return;
 }
}

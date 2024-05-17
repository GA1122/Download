void ACodec::ExecutingState::submitRegularOutputBuffers() {
 bool failed = false;
 for (size_t i = 0; i < mCodec->mBuffers[kPortIndexOutput].size(); ++i) {
 BufferInfo *info = &mCodec->mBuffers[kPortIndexOutput].editItemAt(i);

 if (mCodec->mNativeWindow != NULL) {
 if (info->mStatus != BufferInfo::OWNED_BY_US
 && info->mStatus != BufferInfo::OWNED_BY_NATIVE_WINDOW) {
                ALOGE("buffers should be owned by us or the surface");
                failed = true;
 break;
 }

 if (info->mStatus == BufferInfo::OWNED_BY_NATIVE_WINDOW) {
 continue;
 }
 } else {
 if (info->mStatus != BufferInfo::OWNED_BY_US) {
                ALOGE("buffers should be owned by us");
                failed = true;
 break;
 }
 }

        ALOGV("[%s] calling fillBuffer %u", mCodec->mComponentName.c_str(), info->mBufferID);

        info->checkWriteFence("submitRegularOutputBuffers");
 status_t err = mCodec->mOMX->fillBuffer(mCodec->mNode, info->mBufferID, info->mFenceFd);
        info->mFenceFd = -1;
 if (err != OK) {
            failed = true;
 break;
 }

        info->mStatus = BufferInfo::OWNED_BY_COMPONENT;
 }

 if (failed) {
        mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 }
}

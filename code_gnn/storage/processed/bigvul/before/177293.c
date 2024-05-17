void ACodec::ExecutingState::resume() {
 if (mActive) {
        ALOGV("[%s] We're already active, no need to resume.", mCodec->mComponentName.c_str());
 return;
 }

    submitOutputBuffers();

 if (mCodec->mBuffers[kPortIndexInput].size() == 0u) {
        ALOGW("[%s] we don't have any input buffers to resume", mCodec->mComponentName.c_str());
 }

 for (size_t i = 0; i < mCodec->mBuffers[kPortIndexInput].size(); i++) {
 BufferInfo *info = &mCodec->mBuffers[kPortIndexInput].editItemAt(i);
 if (info->mStatus == BufferInfo::OWNED_BY_US) {
            postFillThisBuffer(info);
 }
 }

    mActive = true;
}

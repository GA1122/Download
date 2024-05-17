void OMXCodec::drainInputBuffers() {
    CHECK(mState == EXECUTING || mState == RECONFIGURING);

 if (mFlags & kUseSecureInputBuffers) {
 Vector<BufferInfo> *buffers = &mPortBuffers[kPortIndexInput];
 for (size_t i = 0; i < buffers->size(); ++i) {
 if (!drainAnyInputBuffer()
 || (mFlags & kOnlySubmitOneInputBufferAtOneTime)) {
 break;
 }
 }
 } else {
 Vector<BufferInfo> *buffers = &mPortBuffers[kPortIndexInput];
 for (size_t i = 0; i < buffers->size(); ++i) {
 BufferInfo *info = &buffers->editItemAt(i);

 if (info->mStatus != OWNED_BY_US) {
 continue;
 }

 if (!drainInputBuffer(info)) {
 break;
 }

 if (mFlags & kOnlySubmitOneInputBufferAtOneTime) {
 break;
 }
 }
 }
}

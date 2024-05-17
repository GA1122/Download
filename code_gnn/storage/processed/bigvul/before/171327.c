void OMXCodec::fillOutputBuffers() {
    CHECK_EQ((int)mState, (int)EXECUTING);

 if (mSignalledEOS
 && countBuffersWeOwn(mPortBuffers[kPortIndexInput])
 == mPortBuffers[kPortIndexInput].size()
 && countBuffersWeOwn(mPortBuffers[kPortIndexOutput])
 == mPortBuffers[kPortIndexOutput].size()) {
        mNoMoreOutputData = true;
        mBufferFilled.signal();

 return;
 }

 Vector<BufferInfo> *buffers = &mPortBuffers[kPortIndexOutput];
 for (size_t i = 0; i < buffers->size(); ++i) {
 BufferInfo *info = &buffers->editItemAt(i);
 if (info->mStatus == OWNED_BY_US) {
            fillOutputBuffer(&buffers->editItemAt(i));
 }
 }
}

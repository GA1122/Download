void ACodec::ExecutingState::submitOutputMetaBuffers() {
 for (size_t i = 0; i < mCodec->mBuffers[kPortIndexInput].size(); ++i) {
 BufferInfo *info = &mCodec->mBuffers[kPortIndexInput].editItemAt(i);

 if (info->mStatus == BufferInfo::OWNED_BY_COMPONENT) {
 if (mCodec->submitOutputMetadataBuffer() != OK)
 break;
 }
 }

    mCodec->signalSubmitOutputMetadataBufferIfEOS_workaround();
}

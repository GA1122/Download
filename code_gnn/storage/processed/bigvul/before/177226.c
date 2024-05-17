status_t ACodec::freeBuffer(OMX_U32 portIndex, size_t i) {
 BufferInfo *info = &mBuffers[portIndex].editItemAt(i);
 status_t err = OK;

 MetadataBufferType type =
        portIndex == kPortIndexOutput ? mOutputMetadataType : mInputMetadataType;
 if (type == kMetadataBufferTypeANWBuffer && info->mData != NULL
 && info->mData->size() >= sizeof(VideoNativeMetadata)) {
 int fenceFd = ((VideoNativeMetadata *)info->mData->data())->nFenceFd;
 if (fenceFd >= 0) {
            ALOGW("unreleased fence (%d) in %s metadata buffer %zu",
                    fenceFd, portIndex == kPortIndexInput ? "input" : "output", i);
 }
 }

 switch (info->mStatus) {
 case BufferInfo::OWNED_BY_US:
 if (portIndex == kPortIndexOutput && mNativeWindow != NULL) {
 (void)cancelBufferToNativeWindow(info);
 }

 case BufferInfo::OWNED_BY_NATIVE_WINDOW:
            err = mOMX->freeBuffer(mNode, portIndex, info->mBufferID);
 break;

 default:
            ALOGE("trying to free buffer not owned by us or ANW (%d)", info->mStatus);
            err = FAILED_TRANSACTION;
 break;
 }

 if (info->mFenceFd >= 0) {
 ::close(info->mFenceFd);
 }

 if (portIndex == kPortIndexOutput) {
        mRenderTracker.untrackFrame(info->mRenderInfo, i);
        info->mRenderInfo = NULL;
 }

    mBuffers[portIndex].removeAt(i);
 return err;
}

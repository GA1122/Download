status_t ACodec::submitOutputMetadataBuffer() {
    CHECK(storingMetadataInDecodedBuffers());
 if (mMetadataBuffersToSubmit == 0)
 return OK;

 BufferInfo *info = dequeueBufferFromNativeWindow();
 if (info == NULL) {
 return ERROR_IO;
 }

    ALOGV("[%s] submitting output meta buffer ID %u for graphic buffer %p",
          mComponentName.c_str(), info->mBufferID, info->mGraphicBuffer.get());

 --mMetadataBuffersToSubmit;
    info->checkWriteFence("submitOutputMetadataBuffer");
 status_t err = mOMX->fillBuffer(mNode, info->mBufferID, info->mFenceFd);
    info->mFenceFd = -1;
 if (err == OK) {
        info->mStatus = BufferInfo::OWNED_BY_COMPONENT;
 }

 return err;
}

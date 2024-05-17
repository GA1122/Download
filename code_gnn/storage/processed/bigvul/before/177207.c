status_t ACodec::cancelBufferToNativeWindow(BufferInfo *info) {
    CHECK_EQ((int)info->mStatus, (int)BufferInfo::OWNED_BY_US);

    ALOGV("[%s] Calling cancelBuffer on buffer %u",
         mComponentName.c_str(), info->mBufferID);

    info->checkWriteFence("cancelBufferToNativeWindow");
 int err = mNativeWindow->cancelBuffer(
        mNativeWindow.get(), info->mGraphicBuffer.get(), info->mFenceFd);
    info->mFenceFd = -1;

    ALOGW_IF(err != 0, "[%s] can not return buffer %u to native window",
            mComponentName.c_str(), info->mBufferID);
    info->mStatus = BufferInfo::OWNED_BY_NATIVE_WINDOW;

 return err;
}

status_t OMXCodec::cancelBufferToNativeWindow(BufferInfo *info) {
    CHECK_EQ((int)info->mStatus, (int)OWNED_BY_US);
    CODEC_LOGV("Calling cancelBuffer on buffer %u", info->mBuffer);
 int err = mNativeWindow->cancelBuffer(
        mNativeWindow.get(), info->mMediaBuffer->graphicBuffer().get(), -1);
 if (err != 0) {
      CODEC_LOGE("cancelBuffer failed w/ error 0x%08x", err);

      setState(ERROR);
 return err;
 }
    info->mStatus = OWNED_BY_NATIVE_WINDOW;
 return OK;
}

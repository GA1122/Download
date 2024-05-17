void OMXCodec::fillOutputBuffer(BufferInfo *info) {
    CHECK_EQ((int)info->mStatus, (int)OWNED_BY_US);

 if (mNoMoreOutputData) {
        CODEC_LOGV("There is no more output data available, not "
 "calling fillOutputBuffer");
 return;
 }

    CODEC_LOGV("Calling fillBuffer on buffer %u", info->mBuffer);
 status_t err = mOMX->fillBuffer(mNode, info->mBuffer);

 if (err != OK) {
        CODEC_LOGE("fillBuffer failed w/ error 0x%08x", err);

        setState(ERROR);
 return;
 }

    info->mStatus = OWNED_BY_COMPONENT;
}

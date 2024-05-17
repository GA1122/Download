void ACodec::BufferInfo::checkWriteFence(const char *dbg) {
 if (mFenceFd >= 0 && mIsReadFence) {
        ALOGD("REUSING read fence %d as write fence in %s", mFenceFd, dbg);
 }
}

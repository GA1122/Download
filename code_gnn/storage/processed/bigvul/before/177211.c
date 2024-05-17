void ACodec::BufferInfo::checkReadFence(const char *dbg) {
 if (mFenceFd >= 0 && !mIsReadFence) {
        ALOGD("REUSING write fence %d as read fence in %s", mFenceFd, dbg);
 }
}

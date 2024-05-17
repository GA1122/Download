void ACodec::BufferInfo::setReadFence(int fenceFd, const char *dbg) {
 if (mFenceFd >= 0) {
        ALOGW("OVERWRITE OF %s fence %d by read fence %d in %s",
                mIsReadFence ? "read" : "write", mFenceFd, fenceFd, dbg);
 }
    mFenceFd = fenceFd;
    mIsReadFence = true;
}

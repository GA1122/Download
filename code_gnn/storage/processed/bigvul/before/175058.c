Sample::Sample(int sampleID, int fd, int64_t offset, int64_t length)
{
    init();
    mSampleID = sampleID;
    mFd = dup(fd);
    mOffset = offset;
    mLength = length;
    ALOGV("create sampleID=%d, fd=%d, offset=%" PRId64 " length=%" PRId64,
        mSampleID, mFd, mLength, mOffset);
}

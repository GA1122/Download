Sample::~Sample()
{
    ALOGV("Sample::destructor sampleID=%d, fd=%d", mSampleID, mFd);
 if (mFd > 0) {
        ALOGV("close(%d)", mFd);
 ::close(mFd);
 }
    free(mUrl);
}

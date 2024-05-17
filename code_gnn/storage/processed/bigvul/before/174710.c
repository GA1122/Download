status_t MediaMetadataRetriever::setDataSource(int fd, int64_t offset, int64_t length)
{
    ALOGV("setDataSource(%d, %" PRId64 ", %" PRId64 ")", fd, offset, length);
 Mutex::Autolock _l(mLock);
 if (mRetriever == 0) {
        ALOGE("retriever is not initialized");
 return INVALID_OPERATION;
 }
 if (fd < 0 || offset < 0 || length < 0) {
        ALOGE("Invalid negative argument");
 return UNKNOWN_ERROR;
 }
 return mRetriever->setDataSource(fd, offset, length);
}

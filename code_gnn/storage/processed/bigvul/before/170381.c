status_t MPEG4DataSource::setCachedRange(off64_t offset, size_t size) {
 Mutex::Autolock autoLock(mLock);

    clearCache();

    mCache = (uint8_t *)malloc(size);

 if (mCache == NULL) {
 return -ENOMEM;
 }

    mCachedOffset = offset;
    mCachedSize = size;

 ssize_t err = mSource->readAt(mCachedOffset, mCache, mCachedSize);

 if (err < (ssize_t)size) {
        clearCache();

 return ERROR_IO;
 }

 return OK;
}

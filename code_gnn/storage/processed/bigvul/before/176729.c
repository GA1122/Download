void Parcel::initState()
{
    LOG_ALLOC("Parcel %p: initState", this);
    mError = NO_ERROR;
    mData = 0;
    mDataSize = 0;
    mDataCapacity = 0;
    mDataPos = 0;
    ALOGV("initState Setting data size of %p to %zu", this, mDataSize);
    ALOGV("initState Setting data pos of %p to %zu", this, mDataPos);
    mObjects = NULL;
    mObjectsSize = 0;
    mObjectsCapacity = 0;
    mNextObjectHint = 0;
    mHasFds = false;
    mFdsKnown = true;
    mAllowFds = true;
    mOwner = NULL;
    mOpenAshmemSize = 0;

 if (gMaxFds == 0) {
 struct rlimit result;
 if (!getrlimit(RLIMIT_NOFILE, &result)) {
            gMaxFds = (size_t)result.rlim_cur;
 } else {
            ALOGW("Unable to getrlimit: %s", strerror(errno));
            gMaxFds = 1024;
 }
 }
}
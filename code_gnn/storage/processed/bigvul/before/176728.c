void Parcel::freeDataNoInit()
{
 if (mOwner) {
        LOG_ALLOC("Parcel %p: freeing other owner data", this);
        mOwner(this, mData, mDataSize, mObjects, mObjectsSize, mOwnerCookie);
 } else {
        LOG_ALLOC("Parcel %p: freeing allocated data", this);
        releaseObjects();
 if (mData) {
            LOG_ALLOC("Parcel %p: freeing with %zu capacity", this, mDataCapacity);
            pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
 if (mDataCapacity <= gParcelGlobalAllocSize) {
              gParcelGlobalAllocSize = gParcelGlobalAllocSize - mDataCapacity;
 } else {
              gParcelGlobalAllocSize = 0;
 }
 if (gParcelGlobalAllocCount > 0) {
              gParcelGlobalAllocCount--;
 }
            pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);
            free(mData);
 }
 if (mObjects) free(mObjects);
 }
}

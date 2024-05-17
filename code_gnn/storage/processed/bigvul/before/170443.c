size_t Parcel::getGlobalAllocSize() {
    pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
 size_t size = gParcelGlobalAllocSize;
    pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);
 return size;
}

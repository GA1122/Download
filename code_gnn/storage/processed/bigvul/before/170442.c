size_t Parcel::getGlobalAllocCount() {
    pthread_mutex_lock(&gParcelGlobalAllocSizeLock);
 size_t count = gParcelGlobalAllocCount;
    pthread_mutex_unlock(&gParcelGlobalAllocSizeLock);
 return count;
}

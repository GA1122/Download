void HeapCache::free_heap(const sp<IBinder>& binder) {
    free_heap( wp<IBinder>(binder) );
}

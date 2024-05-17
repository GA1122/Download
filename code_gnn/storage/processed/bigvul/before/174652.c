 static inline sp<IMemoryHeap> find_heap(const sp<IBinder>& binder) {
 return gHeapCache->find_heap(binder);
 }

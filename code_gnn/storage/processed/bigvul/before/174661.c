 static inline sp<IMemoryHeap> get_heap(const sp<IBinder>& binder) {
 return gHeapCache->get_heap(binder);
 }

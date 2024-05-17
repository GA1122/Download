sp<IMemoryHeap> HeapCache::find_heap(const sp<IBinder>& binder)
{
 Mutex::Autolock _l(mHeapCacheLock);
 ssize_t i = mHeapCache.indexOfKey(binder);
 if (i>=0) {
 heap_info_t& info = mHeapCache.editValueAt(i);
        ALOGD_IF(VERBOSE,
 "found binder=%p, heap=%p, size=%zu, fd=%d, count=%d",
                binder.get(), info.heap.get(),
 static_cast<BpMemoryHeap*>(info.heap.get())->mSize,
 static_cast<BpMemoryHeap*>(info.heap.get())->mHeapId,
                info.count);
        android_atomic_inc(&info.count);
 return info.heap;
 } else {
 heap_info_t info;
        info.heap = interface_cast<IMemoryHeap>(binder);
        info.count = 1;
        mHeapCache.add(binder, info);
 return info.heap;
 }
}

BpMemoryHeap::~BpMemoryHeap() {
 if (mHeapId != -1) {
        close(mHeapId);
 if (mRealHeap) {
 if (mBase != MAP_FAILED) {
                sp<IBinder> binder = const_cast<BpMemoryHeap*>(this)->asBinder();

 if (VERBOSE) {
                    ALOGD("UNMAPPING binder=%p, heap=%p, size=%zu, fd=%d",
                            binder.get(), this, mSize, mHeapId);
 CallStack stack(LOG_TAG);
 }

                munmap(mBase, mSize);
 }
 } else {
            sp<IBinder> binder = const_cast<BpMemoryHeap*>(this)->asBinder();
            free_heap(binder);
 }
 }
}

void BpMemoryHeap::assertMapped() const
{
 if (mHeapId == -1) {
        sp<IBinder> binder(const_cast<BpMemoryHeap*>(this)->asBinder());
        sp<BpMemoryHeap> heap(static_cast<BpMemoryHeap*>(find_heap(binder).get()));
        heap->assertReallyMapped();
 if (heap->mBase != MAP_FAILED) {
 Mutex::Autolock _l(mLock);
 if (mHeapId == -1) {
                mBase   = heap->mBase;
                mSize   = heap->mSize;
                mOffset = heap->mOffset;
                android_atomic_write( dup( heap->mHeapId ), &mHeapId );
 }
 } else {
            free_heap(binder);
 }
 }
}

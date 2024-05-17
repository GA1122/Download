BpMemoryHeap::BpMemoryHeap(const sp<IBinder>& impl)
 : BpInterface<IMemoryHeap>(impl),
        mHeapId(-1), mBase(MAP_FAILED), mSize(0), mFlags(0), mOffset(0), mRealHeap(false)
{
}

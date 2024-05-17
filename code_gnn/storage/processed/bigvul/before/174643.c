BpMemory::BpMemory(const sp<IBinder>& impl)
 : BpInterface<IMemory>(impl), mOffset(0), mSize(0)
{
}

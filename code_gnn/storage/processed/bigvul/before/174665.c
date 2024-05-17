status_t BnMemoryHeap::onTransact(
 uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
 switch(code) {
 case HEAP_ID: {
            CHECK_INTERFACE(IMemoryHeap, data, reply);
            reply->writeFileDescriptor(getHeapID());
            reply->writeInt32(getSize());
            reply->writeInt32(getFlags());
            reply->writeInt32(getOffset());
 return NO_ERROR;
 } break;
 default:
 return BBinder::onTransact(code, data, reply, flags);
 }
}

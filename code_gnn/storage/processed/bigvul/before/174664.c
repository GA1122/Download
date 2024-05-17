status_t BnMemory::onTransact(
 uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
 switch(code) {
 case GET_MEMORY: {
            CHECK_INTERFACE(IMemory, data, reply);
 ssize_t offset;
 size_t size;
            reply->writeStrongBinder( getMemory(&offset, &size)->asBinder() );
            reply->writeInt32(offset);
            reply->writeInt32(size);
 return NO_ERROR;
 } break;
 default:
 return BBinder::onTransact(code, data, reply, flags);
 }
}

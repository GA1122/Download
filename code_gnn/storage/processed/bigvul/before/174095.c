 virtual status_t allowAllocation(bool allow) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(static_cast<int32_t>(allow));
 status_t result = remote()->transact(ALLOW_ALLOCATION, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        result = reply.readInt32();
 return result;
 }

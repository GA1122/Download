 virtual status_t detachBuffer(int slot) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(slot);
 status_t result = remote()->transact(DETACH_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        result = reply.readInt32();
 return result;
 }

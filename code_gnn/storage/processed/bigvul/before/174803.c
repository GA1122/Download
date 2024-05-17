 virtual status_t setDefaultBufferSize(uint32_t w, uint32_t h) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(w);
        data.writeInt32(h);
 status_t result = remote()->transact(SET_DEFAULT_BUFFER_SIZE, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

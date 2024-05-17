 virtual status_t setDefaultBufferSize(uint32_t width, uint32_t height) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeUint32(width);
        data.writeUint32(height);
 status_t result = remote()->transact(SET_DEFAULT_BUFFER_SIZE, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

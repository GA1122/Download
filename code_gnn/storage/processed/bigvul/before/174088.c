 virtual status_t setDefaultMaxBufferCount(int bufferCount) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(bufferCount);
 status_t result = remote()->transact(SET_DEFAULT_MAX_BUFFER_COUNT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

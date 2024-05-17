 virtual status_t setMaxAcquiredBufferCount(int maxAcquiredBuffers) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(maxAcquiredBuffers);
 status_t result = remote()->transact(SET_MAX_ACQUIRED_BUFFER_COUNT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

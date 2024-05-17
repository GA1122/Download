 virtual status_t disableAsyncBuffer() {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
 status_t result = remote()->transact(DISABLE_ASYNC_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

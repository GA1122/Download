 virtual status_t consumerDisconnect() {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
 status_t result = remote()->transact(CONSUMER_DISCONNECT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

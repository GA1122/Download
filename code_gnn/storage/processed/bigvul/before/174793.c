 virtual status_t consumerConnect(const sp<IConsumerListener>& consumer, bool controlledByApp) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeStrongBinder(consumer->asBinder());
        data.writeInt32(controlledByApp);
 status_t result = remote()->transact(CONSUMER_CONNECT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

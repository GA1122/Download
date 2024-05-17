 virtual String8 getConsumerName() const {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
 status_t result = remote()->transact(GET_CONSUMER_NAME, data, &reply);
 if (result != NO_ERROR) {
            ALOGE("getConsumerName failed to transact: %d", result);
 return String8("TransactFailed");
 }
 return reply.readString8();
 }

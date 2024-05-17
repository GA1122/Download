 virtual void setConsumerName(const String8& name) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeString8(name);
        remote()->transact(SET_CONSUMER_NAME, data, &reply);
 }

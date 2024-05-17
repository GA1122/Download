 virtual status_t setDefaultBufferFormat(uint32_t defaultFormat) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(defaultFormat);
 status_t result = remote()->transact(SET_DEFAULT_BUFFER_FORMAT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

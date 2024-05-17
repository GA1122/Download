 virtual status_t setTransformHint(uint32_t hint) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(hint);
 status_t result = remote()->transact(SET_TRANSFORM_HINT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }

 virtual status_t getReleasedBuffers(uint64_t* slotMask) {
 Parcel data, reply;
 if (slotMask == NULL) {
            ALOGE("getReleasedBuffers: slotMask must not be NULL");
 return BAD_VALUE;
 }
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
 status_t result = remote()->transact(GET_RELEASED_BUFFERS, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 *slotMask = reply.readInt64();
 return reply.readInt32();
 }

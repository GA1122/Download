 virtual status_t setBufferCount(int bufferCount)
 {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(bufferCount);
 status_t result =remote()->transact(SET_BUFFER_COUNT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        result = reply.readInt32();
 return result;
 }

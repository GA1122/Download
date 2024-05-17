 virtual status_t disconnect(int api) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(api);
 status_t result =remote()->transact(DISCONNECT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        result = reply.readInt32();
 return result;
 }

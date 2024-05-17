 virtual int query(int what, int* value) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(what);
 status_t result = remote()->transact(QUERY, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        value[0] = reply.readInt32();
        result = reply.readInt32();
 return result;
 }

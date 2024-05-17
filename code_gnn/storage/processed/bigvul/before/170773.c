 virtual status_t initAsync(const char *host, unsigned port) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        data.writeCString(host);
        data.writeInt32(port);
        remote()->transact(HDCP_INIT_ASYNC, data, &reply);
 return reply.readInt32();
 }

 virtual status_t shutdownAsync() {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        remote()->transact(HDCP_SHUTDOWN_ASYNC, data, &reply);
 return reply.readInt32();
 }

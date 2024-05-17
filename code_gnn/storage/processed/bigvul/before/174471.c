 virtual status_t releaseAllSecureStops() {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

 status_t status = remote()->transact(RELEASE_ALL_SECURE_STOPS, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }

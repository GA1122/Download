 virtual status_t releaseSecureStops(Vector<uint8_t> const &ssRelease) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, ssRelease);
 status_t status = remote()->transact(RELEASE_SECURE_STOPS, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }

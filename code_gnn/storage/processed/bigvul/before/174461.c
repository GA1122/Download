 virtual status_t getSecureStop(Vector<uint8_t> const &ssid, Vector<uint8_t> &secureStop) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, ssid);
 status_t status = remote()->transact(GET_SECURE_STOP, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, secureStop);
 return reply.readInt32();
 }

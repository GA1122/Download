 virtual status_t getSecureStops(List<Vector<uint8_t> > &secureStops) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

 status_t status = remote()->transact(GET_SECURE_STOPS, data, &reply);
 if (status != OK) {
 return status;
 }

        secureStops.clear();
 uint32_t count = reply.readInt32();
 for (size_t i = 0; i < count; i++) {
 Vector<uint8_t> secureStop;
            readVector(reply, secureStop);
            secureStops.push_back(secureStop);
 }
 return reply.readInt32();
 }
